from termcolor import colored

# print colored (string, 'yellow', attrs=['bold'])
def RED(string):
    toprint = colored (string, 'red')
    return (toprint)

def BLUE(string):
    toprint = colored (string, 'blue')
    return (toprint)

def GREEN(string):
    toprint = colored (string, 'green')
    return (toprint)

def YELLOW(string):
    toprint = colored (string, 'yellow')
    return (toprint)

def WHITE_BOLD(string):
    toprint = colored (string, 'white', attrs=['bold'])
    return (toprint)

def RED_ALERT(string):
    return (colored ('[!] ', 'red') + string)

def BLUE_ALERT(string):
    return (colored ('[x] ', 'blue') + string)

def GREEN_ALERT(string):
    return (colored ('[+] ', 'green') + string)

def YELLOW_ALERT(string):
    return (colored ('[*] ', 'yellow') + string)

def WHITE_BOLD_ALERT(string):
    return (colored (string, 'white', attrs=['bold']))
