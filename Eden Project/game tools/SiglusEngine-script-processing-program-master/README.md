# SiglusEngine-script-processing-program

In the process of script processing for SiglusEngine, we encountered many situations that required editing with text processing programs, such as the need to put character names and dialogues on one line, and to filter out program command lines in scripts, etc. We have now completed a more complete script processing program for SiglusEngine only, and we can later use this program for many This program can be used to process scripts of many engines.

This program has only one main.cpp that makes sense, which has a total of four function functions and three call functions for easy execution, which are implemented as follows.

Deal_Txt(): the script file is processed, and the character name and dialogue content are put on one line by judging the left bracket, while all the program instruction lines are filtered out by judging whether the first character is an English character or not, and the result of the execution is that the script file is split into two files, the first one is a text-only translation file for convenient translation, and the second one is used to save the program instruction and its line number The first is a text-only translation file for easy translation, and the second is an additional file for saving the program instructions and their lines.

Restore_Txt(): Restore the two files just generated, and reassemble them into the original script file, but the text part of it has been translated, the new script file generated has no formatting difference with the original script file, and can be directly used for the upper-level package operation.

Change_Name(): use rename function to modify the file name in batch, here is mainly for Siglus engine related unpacking tools to generate the extended suffix problem, this program can set the file suffix length to delete the number of bits, for example, ".ss.txt.out", delete to direct out" to ".ss.txt" which can be packaged directly. However, rename can also perform other complex operations, and can be modified flexibly if needed.

The *_Do function is the entry point for executing the above functions. The Do series of functions have a file loop that can operate on all script files in the entire folder.

在对SiglusEngine的脚本处理过程中，遇到了许多需要使用文本处理程序进行编辑的情况，例如需要将角色名称与对话放在一行，以及过滤掉脚本中的程序指令行等等，目前已经完成了一个只针对SiglusEngine的较为完善的脚本处理程序，之后可以基于本程序对诸多引擎的脚本进行处理。

本程序只有一个main.cpp有意义，其中一共有四个功能函数，三个方便执行的调用函数，分别实现的功能如下：

Deal_Txt()：将脚本文件进行处理，通过判定左括号将角色姓名和对话内容放在一行，同时通过判断首个字符是不是英文字符来过滤掉全部的程序指令行，执行的结果是将脚本文件拆成两个文件，第一个是方便翻译的只有文本的翻译文件，第二个是用于保存程序指令及其行数的附加文件。

Restore_Txt()：将刚才生成的两个文件进行复原，重新组合为原始脚本文件，但是其中的文本部分已经被翻译，生成的新脚本文件和原始脚本文件没有格式上的差别，可以直接进行上层的封包操作。

Change_Name()：使用rename函数批量修改文件名称，这里主要是针对Siglus引擎相关解包工具生成的延长后缀问题，本程序可以设置文件后缀长度删减位数，例如将“.ss.txt.out”，删减为可以直接打包的“.ss.txt”。但是rename也可以进行其他复杂操作，如果有需要可以灵活修改。

*_Do函数都是执行以上函数的入口，Do系列函数内有文件循环操作，可以对整个文件夹的所有脚本文件进行操作。
