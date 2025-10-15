# YRDICT

## 前言/PREFACE

YRDict 是一个旨在整合大家对红色警戒2：尤里的复仇（YR）的逆向成果与理解的项目。
这个项目和YRpp互为表里，YRpp提供接口，这个项目记录西木工作室（WW）的实际实现。
希望大家把整理的函数、逆向的心得和碰到的WWSB之处记录下来。
同时，如果有什么想法，都可以写在这个项目当中。


YRDict aims to integrate and document the community's 
reverse engineering efforts and insights for Yuri's Revenge (YR).
This project and YRpp are designed to complement each other: 
YRpp provides the interfaces, while this project documents the actual implementations by Westwood (WW).
We encourage you to record disassembled functions, 
reverse engineering notes, and any "WWSB" (WW's unique wisdom) you encounter.
Also, if you have any ideas, feel free to write them in this project.

## 整理方式/ORGANIZATION

YRDict 以函数为单位进行整理。
每个函数在YRpp等位置记录接口和实际的地址。
同时，利用DynamicPatch库钩下原有位置的整个函数，并在YRDict中记录实现。

如果没有完整的实现，也记录下来部分函数，其余部分用IDA产出的代码暂时补充。
如果不能够稳定工作，也可以记录下来，但暂时不钩原函数。
如果有一点点的心得和注意事项，也烦请留下。
无论有什么进展，请在CHANGELOG.md当中标明。

这个仓库容纳的不仅仅是代码，“能用的实现”“整理的代码片段”“单纯的心得”都欢迎。
不要拘泥于形式，能帮助到后人就好。
也不要担心无法使用，只是一个力图详尽的记录。
所谓海纳百川，也请多多丰富一下这个库的体例和形制。

在编写代码和记录心得之外，把相应的实现.h/.cpp等
放在目录下面的合适位置，并适当的取名。
对单个类的实现，在YRpp当中记录下这个类的Type和具体内容，
同时，在实现当中以一个Type_Impl来容纳实现。


YRDict is organized by functions. For each function, 
its interface and memory address are documented in projects like YRpp.
At the same time, the original function is hooked in place using the DynamicPatch library, 
and its full implementation is documented here in YRDict.

Even without a complete implementation, partial functions should be documented, 
with missing portions temporarily supplemented by IDA-generated code.
If it cannot work stably, it can also be documented,
but the original function is not hooked for the time being.
If there are any insights or precautions, please leave them as well.
Whatever progress you make, please specify it in CHANGELOG.md.

This repository accommodates not only code,
but also "usable implementations", "organized code snippets", and "pure insights" are all welcome.
Don't worry too much about the format; 
the primary goal is to provide value and clarity for future contributors.
Also, don't worry about usability; 
it's just an attempt at a comprehensive record.
The idea is to be inclusive, 
so please enrich the format and structure of this repository.

In addition to writing code and documenting insights,
place the corresponding implementation .h/.cpp files in the appropriate locations under the directory,
and name them appropriately.
For the implementation of a single class,
record the Type and specific content of this class in YRpp,
and at the same time, use a Type_Impl to contain the implementation.

## 使用/USAGE

把这个项目编译成DLL，或下载Release当中的DLL。
把DLL放在游戏目录下，同时保证没有任何其他的引擎扩展。
（包括Ares/Phobos/Kratos/DynamicPatcher/SIWinterIsComing等）
之后使用Syringe启动即可。

请务必确保游戏目录中只有YRDict一个引擎扩展。
与其他扩展同时加载会导致不可预测的行为。

当然，也可以仅仅拿来参考，或者用来作为自己扩展的基础。
这个项目的内容是开源的，遵循MIT许可证。


Compile this project into a DLL, or download the DLL from the Release page.
Place the DLL in the game directory,
ensuring that there are no other engine extensions.
(This includes Ares/Phobos/Kratos/DynamicPatcher/SIWinterIsComing, etc.)
Then launch the game using Syringe.

Please ensure that YRDict is the only engine extension in the game directory.
Loading it alongside other extensions may lead to unpredictable behavior.

Naturally, you are also free to use this project 
solely for reference or as a foundation for your own extensions.
The content of this project is open source and follows the MIT license.

## 注意/NOTE

这个项目的目的不是完整逆向YR，而只是记录较为重要的函数与实现。
如果你有兴趣，可以尝试补充更多的函数。
这个项目也是希望大家能够抽丝剥茧，逐步理解整个引擎在如何工作。

这个项目可以修正WW的原有BUG，但请注明WW原来是怎么写的。
如果有意愿的话，也可以标注出Ares/Phobos等钩了什么位置以及改变了什么。
不考虑提供任何的新增或增强逻辑。
允许也欢迎添加与调试等相关的辅助功能。

所有人注意不要直接提交到main分支。
可以申请加入，并在dev分支上直接开发与提交；
也可以fork本项目，并在自己的分支当中添加内容。
以上两种方式都应通过Pull Request的方式合并到main分支。


Please note that this project does not aim to fully reverse engineer YR. 
Instead, it focuses on documenting critical functions and implementations.
If you are interested, you can try to supplement more functions.
A secondary goal is to help the community piece together 
a comprehensive understanding of how the game engine operates.

This project can fix the original bugs of WW, 
but please indicate how WW was originally written.
If you are willing, you can also indicate what positions 
Ares/Phobos, etc. hooked and what changes they made.
No new or enhanced logic is considered.
Adding auxiliary features related to debugging and similar tasks is allowed and encouraged.

Everyone, please avoid committing directly to the main branch.
You can apply to join and develop and commit directly on the dev branch;
or you can fork this project and add content in your own branch.
Both methods should merge into the main branch via Pull Request.

## 结语/CONCLUSION

你的贡献会为每一个后人点亮一盏灯。
我们相信，道阻且长，行则将至。


Your contributions will light the way for those who follow.
We believe that while the road ahead is long, 
every step forward brings us closer.