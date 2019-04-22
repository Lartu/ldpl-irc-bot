# 🤖 IRCBot.ldpl
A super easy to use IRC bot library for LDPL. It lets you write LDPL programs that connect to IRC servers, join channels,
send messages and more in the simplest way possible.

## How to set up
To use this library you must have an [**LDPL** compiler](http://github.com/lartu/ldpl) newer than or equal to **LDPL 3.0.4**
(this library uses the `EXTERNAL SUB-PROCEDURE` statement introduced in said version).

To add IRCBot.ldpl to your LDPL project you must include `ircbot.cpp` in your compilation line like this:

`$ ldpl -i=ircbot.cpp myLdplSource.ldpl`

## Example and documentation
The file `ircbot.ldpl` is an example of how to use this library. The library is also documented within that file.

To compile `ircbot.ldpl` run `$ ldpl -i=ircbot.cpp ircbot.ldpl -o=ircbot`.

## License
IRCBot.ldpl is heavily based on [**SirLogsalot**](https://github.com/gkbrk/SirLogsalot)
by [Gokberk Yaltirakli](https://github.com/gkbrk). His code is released under the *MIT LICENSE*
and so is this library.
