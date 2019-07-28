![The LDPL IRC Bot Library](images/ldpl-irc-logo.png)

**The LDPL IRC Bot Library** is a super easy to use IRC bot library for [LDPL](https://www.github.com/lartu/ldpl).
It lets you write LDPL programs that connect to IRC servers, join channels, send messages and more in the simplest
way possible. This library requires **LDPL 3.0.4** or newer.

## How to set up
To add IRCBot.ldpl to your LDPL project you must include `ircbot.cpp` in your compilation line like this:

`$ ldpl -i=ircbot.cpp myLdplSource.ldpl`

Include the library into your LDPL project by copying the file *ldpl_irc_bot.cpp*
to your project directory and then adding the line:

`EXTENSION "ldpl_irc_bot.cpp"`

## Example and documentation
The file `ldpl_irc_bot.ldpl` is an example of how to use this library. The library is also documented within that file.

## License
The LDPL IRC Bot Library is heavily based on [**SirLogsalot**](https://github.com/gkbrk/SirLogsalot)
by [Gokberk Yaltirakli](https://github.com/gkbrk). His code is released under the *MIT LICENSE*
and so is this library.
