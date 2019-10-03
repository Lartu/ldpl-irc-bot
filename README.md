![The LDPL IRC Bot Library](images/ldpl-irc-logo.png)

**The LDPL IRC Bot Library** is a super easy to use IRC bot library for [LDPL](https://www.github.com/lartu/ldpl).
It lets you write LDPL programs that connect to IRC servers, join channels, send messages and react to received messages in the simplest
way possible. This library requires **LDPL 3.0.4** or greater.

## Usage

Include the library into your LDPL project by copying the files *ldpl_irc_bot.cpp*
and *ldpl_irc_bot.ldpl* to your project directory and then adding the line:

`INCLUDE "ldpl_irc_bot.ldpl"`

before the `DATA` and `PROCEDURE` sections of your main project file. The library is
ready to be used.

## Example and documentation
The file `ldpl_irc_bot.ldpl` is an example of how to use this library. The library is also documented within that file.

## License
The LDPL IRC Bot Library is heavily based on [**SirLogsalot**](https://github.com/gkbrk/SirLogsalot)
by [Gokberk Yaltirakli](https://github.com/gkbrk). His code is released under the *MIT LICENSE*
and so is this library.
