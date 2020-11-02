"""""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """
    Copyright (C) 2019 LCIS Laboratory - Cyril Bresch
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
        the Free Software Foundation, in version 3.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
    This program is part of the SecPump @https://github.com/r3glisss/SecPump

""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """""" """"""
from termcolor import colored


def RED(string):
    toprint = colored(string, 'red')
    return (toprint)


def BLUE(string):
    toprint = colored(string, 'blue')
    return (toprint)


def GREEN(string):
    toprint = colored(string, 'green')
    return (toprint)


def YELLOW(string):
    toprint = colored(string, 'yellow')
    return (toprint)


def WHITE_BOLD(string):
    toprint = colored(string, 'white', attrs=['bold'])
    return (toprint)


def RED_ALERT(string):
    return (colored('[!] ', 'red') + string)


def BLUE_ALERT(string):
    return (colored('[x] ', 'blue') + string)


def GREEN_ALERT(string):
    return (colored('[+] ', 'green') + string)


def YELLOW_ALERT(string):
    return (colored('[*] ', 'yellow') + string)


def WHITE_BOLD_ALERT(string):
    return (colored(string, 'white', attrs=['bold']))
