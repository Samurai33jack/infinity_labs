import hashlib
import time
import itertools, string

def wordlist(m,pattern=None):
    if pattern is None:
        pattern = '%'*m
    letters = string.ascii_lowercase
    upper = string.ascii_uppercase
    spec = '#@&$%*()+'
    number = '1234567890'
    upper = upper + number
    info = {'@':spec,'^':upper,'%':letters,'*':number}
    chars = [info.get(d,letters) for d in pattern]
    return [''.join(p) for p in itertools.product(*chars)]


password = "zzzzzz"
password_pattern = '%%%%%%'
password_length = len(password)
print(f"password_length is {password_length}")
wordlist_path = "/home/jakobshnider/wordlist/rockyou.txt"
hash_object = hashlib.sha1(password.encode('utf-8'))
hex_dig = hash_object.hexdigest()

print(f"Hashed String: {hex_dig}")

start_time = time.time()
#with open(wordlist_path, encoding="utf-8", errors="ignore") as wordlist_file:
    # wordlist = wordlist_file.readlines()
cur_wordlist = wordlist(password_length, password_pattern)

for word in cur_wordlist:
    word = word.rstrip()
    cur_hash = hashlib.sha1(word.encode('utf-8'))
    cur_dig = cur_hash.hexdigest()
    if cur_dig == hex_dig:
        print(f"the word is: {word}")
        break

elapsed_time = round(time.time() - start_time, 2)
print(f"the elapsed time is: {elapsed_time}")