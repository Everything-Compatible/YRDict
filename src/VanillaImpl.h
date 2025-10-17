#pragma once

/*
以下的开关均通过 if constexpr 在编译期决定。
The following switches are all decided at compile time via if constexpr.
*/

/*
使用默认实现。
默认为true来尽量使用原版实现。
切换为false以禁用原版实现。

Default - Use Vanilla Implementation
Default to true to use vanilla implementations where possible.
Switch to false to disable vanilla implementations
*/
inline const bool UseVanillaImpl = true;

/*
在Bug修复上使用默认实现
默认为false来修复Bug。
切换为true来使用有Bug的原版实现。
注意：Bug修复应与对应的原版实现同时发布。

Bugfix - Use Vanilla Implementation for Bugfixes
Default to false to fix bugs.
Switch to true to use vanilla implementations even if they have bugs.
NOTE: Bugfixes should be released along with the Vanilla Implementation .
*/
inline const bool UseVanillaImpl_Bugfix = false;

/*
在补充功能上使用默认实现
默认为false以启用日志等功能。
切换为true来停用这些功能。

Supplementary - Use Vanilla Implementation for Supplementary Functions
Default to false to enable supplementary functions (Logging etc.)
Switch to true to turn off Supplementary functions.
*/
inline const bool UseVanillaImpl_Supplementary = false;

/*
在关键位置使用默认实现。
默认为false来重写关键功能，
否则游戏可能无法正常启动。

Critical - Use Vanilla Implementation for Critical Functions 
Default to false since some critical functions have to be rewritten,
or the game cannot be launched properly.
*/
inline const bool UseVanillaImpl_Critical = false;