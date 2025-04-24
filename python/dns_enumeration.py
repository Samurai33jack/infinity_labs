import socket

import requests


def dns_enumeration(url, words_list):
    new_urls = []
    for word in words_list:
        new_url = word.rstrip() + '.' + url
        new_urls.append(new_url)

    counter = 0
    for url in new_urls:
        try:
            counter += 1
            res = socket.gethostbyname(url)
            print("test number " + str(counter))
            print(f"the ip of {url} is {res}")
        except Exception as e:
            pass


def main():
    with open('words_list.txt', 'r') as words_list:
        dns_enumeration('google.com', words_list=words_list)


if __name__ == "__main__":
    main()
