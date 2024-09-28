## General:

The protocol is <u>**entirely string based**</u> and only takes Numbers as a decimal String.

### Valid user input:

Only `ascii`-characters in the range of `(SPACE)0x20-(~)(0x7E)` and `(\n)0x0A` are valid user input

Newline ends a command... (to write newline in a message send `"\n"`)

### Notation:

` command <arg> -> retval` 

## `register`/`login` (Initialize a session)

`register <uname> <passwd> -> SessionID`

`login <uname> <passwd> -> SessionID`

#### Description:

Both command initialize a session.

`login` checks if the provided `uname` and the provided `passwd` match in the `users.db`.

`register` creates a new account by adding a new entry to the `users.db`.

​	 (If a user with the given `uname` already exists `Fail` is returned.)

#### Arguments:

`register` and `login` both take a user-name (`uname`) and a password (`passwd`).



#### Returns:

Server sends back a Session-ID-token (`SessionId`) (random 8 Byte number) 
or `Fail` on error (Invalid credentials).



## `exit` (Close a session)

`exit <SessionID> -> Ack`

#### Description:

Closes a session.

#### Arguments:

Takes a Session-ID-token (`SessionID`) and invalidates it.

(It is guaranteed that this Token will not be used again till the `usedTokens` file is deleted.)

#### Returns:

The server Sends Back a Single `Ack`.

## `help` (sends command list with description)

## `list` (lists all chats of the user)

## `list-all` (lists all users that set there profile to public)

## `request` (request a chat between another user)

## `remove` (remove a chat and private chat with someone)

## `send` (send a text message)

## `read` (opens a chat to a specific user)

## `search` (search a chat for a specific key word)

## `emojis` (send a list of ASCII-art emojis)

# File-system-structure

```
.achat_data
├── active_sessions
├── usedTokens
├── users.db
└── Chats
    ├── <user1> <user2> # user1 is lexicographically smaler than user2 (sepaerated by space)
    └── ...

```



# Files:

## `active_sessions`

### File structure

```
<token>\t<uname>
<token>\t<uname>
...
```

## `usedTokens`

`usedTokens` is a list of Tokens the server already used.

Using the following scheme:

### File structure

````
<token>
<token>
...
````

## `users.db`

### File structure
```
<uname>\t<passwd>\t<chat1>\t<chat2>
<uname>\t<passwd>\t<chat1>\t<chat2>
...
```

## `chat file` (chat)

### File structure

```
----------------------------------
<date><time><uname>
<msg>
----------------------------------
<date><time><uname>
<msg>
----------------------------------
...
```
