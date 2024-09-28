#!/usr/bin/env python3

from types import new_class
from ctf_gameserver import checkerlib
from random import choice
from string import ascii_letters, ascii_uppercase
import pwn
import logging
import utils

pwn.context.log_level='critical'
pwn.context.timeout = 15

def rand_string(length):
    return ''.join(choice(ascii_letters) for i in range(length))

def send_and_recv(ip, msg):
    logging.info(f"rqst = {msg}, ip = {ip}")
    p = pwn.remote(ip, 1337)
    p.sendlineafter(b"$", bytes(msg, 'ascii'))
    p.shutdown()
    resp = p.recvall().decode()[:-2]

    p.close()

    logging.info(f"resp = {resp}")
    return resp

class achatChecker(checkerlib.BaseChecker):
    def place_flag(self, tick: int):
        uname = rand_string(0x10)
        passwd = rand_string(0x20)
        sessionID = ""
        # register user
        logging.info(f"register user (place flag)")
        for _ in range(3):
            try:
                resp = send_and_recv(self.ip, f"register {uname} {passwd}".format())
            except:
                return checkerlib.CheckResult.DOWN
            if "Invalid username" in resp:
                return checkerlib.CheckResult.FAULTY
            if "SessionID: " in resp:
                sessionID = resp[len("SessionID: "):-1]
                break
            if "already exists" not in resp:
                return checkerlib.CheckResult.FAULTY
            uname = rand_string(0x10)
        
        # start-chat <session-id> <uname>
        logging.info(f"start-chat (place flag)")
        resp = send_and_recv(self.ip, f"start-chat {sessionID} {uname}".format())
        if "Created the chat" not in resp:
            return checkerlib.CheckResult.FAULTY
        # save state
        checkerlib.set_flagid(uname)
        checkerlib.store_state(str(tick), {'uname':uname, 'passwd': passwd, 'sessionID':sessionID})

        # send <session-id> <uname&uname> <falg>
        logging.info(f"send (place flag)")
        resp = send_and_recv(self.ip, f"send {sessionID} {uname}&{uname} {checkerlib.get_flag(tick)}".format())
        if "send the Message:" not in resp:
            return checkerlib.CheckResult.FAULTY
        
        return checkerlib.CheckResult.OK


    def check_flag(self, tick: int):
        state = checkerlib.load_state(str(tick))
        # read chat of uname&uname
        logging.info(f"read (check flag)")
        try:
            resp = send_and_recv(self.ip, f"read {state['sessionID']} {state['uname']}&{state['uname']}".format())
        except:
            return checkerlib.CheckResult.FLAG_NOT_FOUND

        if checkerlib.get_flag(tick) not in resp:
            return checkerlib.CheckResult.FLAG_NOT_FOUND
        elif "Invalid SessionID: " in resp or "Invalid chat(" in resp or "does not exist!" in resp:
            return checkerlib.CheckResult.FLAG_NOT_FOUND
        return checkerlib.CheckResult.OK



    def check_service(self):
        uname = "check"+rand_string(16)
        passwd = rand_string(6)
        sessionID = ""

        # register user
        logging.info("regiser (check service)")
        for _ in range(3):
            try:
                resp = send_and_recv(self.ip, f"register {uname} {passwd}".format())
            except:
                return checkerlib.CheckResult.DOWN
            if "Invalid username" in resp:
                return checkerlib.CheckResult.FAULTY
            if "SessionID: " in resp:
                sessionID = resp[len("SessionID: "):-1]
                break
            if "already exists" not in resp:
                return checkerlib.CheckResult.FAULTY
            uname = rand_string(0x10)
        
        # start-chat <session-id> <uname>
        logging.info("start-chat (check service)")
        resp = send_and_recv(self.ip, f"start-chat {sessionID} {uname}".format())
        if "Created the chat" not in resp:
            return checkerlib.CheckResult.FAULTY

        # send <session-id> <uname&uname> <falg>
        logging.info("send (check service)")
        test_msg = "Hello, there!" + rand_string(0x50)
        resp = send_and_recv(self.ip, f"send {sessionID} {uname}&{uname} {test_msg}".format())
        if "send the Message:" not in resp:
            return checkerlib.CheckResult.FAULTY

        # search <session-id> test_msg[:7]
        logging.info("search (check service)")
        resp = send_and_recv(self.ip, f"search {sessionID} {test_msg[:7]}".format())
        if test_msg not in resp:
            return checkerlib.CheckResult.FAULTY

        # list <Session-id>
        logging.info("list (check service)")
        resp = send_and_recv(self.ip, f"list {sessionID}".format())
        if f"{uname}&{uname}".format() not in resp:
            return checkerlib.CheckResult.FAULTY
        
        # list-users <Session-ID>
        # logging.info("list-users (check service)")
        # resp = send_and_recv(self.ip, f"list-users {sessionID}".format())
        # if uname not in resp:
        #     return checkerlib.CheckResult.FAULTY


        return checkerlib.CheckResult.OK


if __name__ == '__main__':
    checkerlib.run_check(achatChecker)
