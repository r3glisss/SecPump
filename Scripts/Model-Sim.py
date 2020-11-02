#############################################################################
# Copyright (C) 2019 LCIS Laboratory - Cyril Bresch
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, in version 3.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
# This program is part of the SecPump @https://github.com/r3glisss/SecPump
#############################################################################
import random
import numpy as np
import matplotlib.pyplot as plt

from scipy.integrate import odeint


# Global configurations
# PID Tuning
Kc_slide = -0.07

tauI_slide = 1
tauD_slide = 1.2
bias_slide = 0.0
p4_slide = 0.05

disturb_box = True

P_box = True
I_box = True
D_box = True


def diabetic(y, t, ui, d, p4):
    # Expanded Bergman Minimal model to include meals and insulin
    # Parameters for an insulin dependent type-I diabetic

    g = y[0]  # Blood glucose (mg/dL)
    x = y[1]  # Remote insulin (1/min)
    i = y[2]  # Insulin (micro-U/L)

    # Parameters:
    gb = 280.0  # Basal Blood Glucose (mg/dL)
    p1 = 0.028735  # 1/min
    p2 = 0.028344  # 1/min
    p3 = 5.035e-5  # L/(min2 * mU)
    # p4    = 0.05
    ib = 0
    si = 2.9e-2  # 1/min * (mL/micro-U)
    vi = 12.0  # L

    # Compute ydot:
    dydt = np.empty(3)

    dydt[0] = -p1 * (g - gb) - x * g + d  # Blood Glucose G
    dydt[1] = -p2 * x + p3 * (i - ib)  # Remote insulin compartment dynamics X
    dydt[2] = -p4 * i + ui / vi  # Insulin dynamics I

    # Convert from minutes to hours
    dydt = dydt * 60
    return dydt


# @params PID tuning
def pidPlot(Kc, tauI, tauD, bias, p4, disturb, eP, eIT, eD):
    # Initial Conditions
    # G,X,I
    y0 = np.array([280, 0, 0])

    # Initial Steady State Condition for the Control
    u_ss = 0  # mU/min

    # Steady State for the Disturbance
    d_ss = 0  # mg/dL.min

    # Final Time (hr)
    tf = 24  # simulate for 24 hours
    ns = tf * 6 + 1  # sample time = 10 min

    # Time Interval (min)
    t = np.linspace(0, tf, ns)

    # Store results for plotting
    G = np.ones(len(t)) * y0[0]
    X = np.ones(len(t)) * y0[1]
    I = np.ones(len(t)) * y0[2]
    u = np.ones(len(t)) * u_ss
    d = np.ones(len(t)) * d_ss

    # Create plot
    figure = plt.figure(figsize=(10, 7))
    figure.show()
    figure.canvas.draw()

    # Disturbance Trigering
    t0 = 0

    # Disturbance Factor
    B = 0

    # Storage for recording values
    op = np.ones(ns + 1) * bias  # controller output
    pv = np.zeros(ns + 1)  # process variable
    e = np.zeros(ns + 1)  # error
    ie = np.zeros(ns + 1)  # integral of the error
    dpv = np.zeros(ns + 1)  # derivative of the pv
    P = np.zeros(ns + 1)  # proportional
    IT = np.zeros(ns + 1)  # integral
    D = np.zeros(ns + 1)  # derivative
    sp = np.ones(ns + 1) * 85  # set point

    # Upper and Lower limits on OP
    op_hi = 100.0
    op_lo = 0.0

    # Number of ticks
    ticks = np.linspace(0, 24, 13)

    # Log file definition
    log_file = open("artificial_pancreas.log", "w")

    # Type-I Diabetic Blood Glucose Simulation
    for i in range(len(t) - 1):
        # Meal disturbance a 8,14, and 20
        if disturb:
            if i == (8 * 6 + 1) or i == (13 * 6 + 1) or i == (19 * 6 + 1):
                t0 = i
                B = random.randint(1, 10)

        # Date of mg glucose pr. dL entering the blood after perturbation
        # defined by d(t) = B * exp(-drate * t) B=9 and drate=0.05
        d[i + 1] = B * np.exp(-0.05 * (i - t0))

        ts = [t[i], t[i + 1]]

        # PID worker
        delta_t = t[i + 1] - t[i]

        pv[i] = G[i]
        e[i] = sp[i] - pv[i]

        if i >= 1:  # Computation starts at second cycle
            dpv[i] = (pv[i] - pv[i - 1]) / delta_t
            ie[i] = ie[i - 1] + e[i] * delta_t

        P[i] = Kc * e[i]
        IT[i] = Kc / tauI * ie[i]
        D[i] = -Kc * tauD * dpv[i]

        # PID activation
        if eP:
            op[i] += P[i]
        if eIT:
            op[i] += IT[i]
        if eD:
            op[i] += D[i]

        # Anti-reset windup system
        if op[i] > op_hi:  # check upper limit
            op[i] = op_hi
            ie[i] = ie[i] - e[i] * delta_t  # anti-reset windup

        if op[i] < op_lo:  # check lower limit
            op[i] = op_lo
            ie[i] = ie[i] - e[i] * delta_t  # anti-reset windup

        u[i + 1] = op[i]

        y = odeint(diabetic, y0, ts, args=(u[i + 1], d[i + 1], p4))
        G[i + 1] = y[-1][0]
        X[i + 1] = y[-1][1]
        I[i + 1] = y[-1][2]
        y0 = y[-1]

        ax = plt.subplot(2, 2, 1)
        plt.plot(t[0:i + 1], u[0:i + 1], 'b--', linewidth=3)
        plt.ylabel('mU/min')
        plt.xlabel('Time (hr)')
        plt.legend(['U(t)'], loc='best')
        plt.xlim([0, 24])
        plt.xticks(ticks)

        ax = plt.subplot(2, 2, 2)
        plt.plot(t[0:i + 1], G[0:i + 1], 'r-', linewidth=3, label='G(t)')
        plt.plot([0, 24], [85, 85], 'k--', linewidth=2, label='Target')
        plt.plot([0, 24], [60, 60], 'k:', linewidth=2, label='Limits')
        plt.plot([0, 24], [150, 150], 'k:', linewidth=2, label='Limits')
        plt.ylabel('mg/dL')
        plt.xlabel('Time (hr)')
        plt.legend(['G(t)', 'Target', 'Limits'], loc='best')
        plt.xlim([0, 24])
        plt.xticks(ticks)

        ax = plt.subplot(2, 2, 3)
        plt.plot(t[0:i + 1], I[0:i + 1], 'k.-', linewidth=3, label='I(t)')
        plt.ylabel('mU/L')
        plt.xlabel('Time (hr)')
        plt.xlim([0, 24])
        plt.legend(['I(t)'], loc='best')
        plt.xticks(ticks)

        ax = plt.subplot(2, 2, 4)
        plt.plot(t[0:i + 1], X[0:i + 1], 'g:', linewidth=3, label='X(t)')
        plt.ylabel('1/min')
        plt.xlabel('Time (hr)')
        plt.xlim([0, 24])
        plt.legend(['X(t)'], loc='best')
        plt.xticks(ticks)

        # Logging
        log = "[u]:" + str(u[i + 1]) + ";"
        log += "[G]:" + str(G[i + 1]) + ";"
        log += "[I]:" + str(I[i + 1]) + ";"
        log += "[X]:" + str(X[i + 1])
        print log
        log_file.write(log + "\n")

        # Update display
        figure.canvas.draw()


pidPlot(Kc=Kc_slide,
        tauI=tauI_slide,
        tauD=tauD_slide,
        bias=bias_slide,
        p4=p4_slide,
        disturb=disturb_box,
        eP=P_box,
        eIT=I_box,
        eD=D_box)
