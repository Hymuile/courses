package com.exam;

import java.io.File;

/**
 * @author 24206
 */
public class Const {

    static File sourceFile;
    static File targetFile;
    static final char EOF = (char)-1;

    public enum TokenType {
        //记号
        ENDFILE, ERROR,
        // 保留字
        IF, ELSE, INT, RETURN, VOID, WHILE,
        // 标识符和数字
        ID, NUM,
        // 特殊符号
        ASSIGN, EQ, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN,
        SEMI, MORE, LESS, COMMA, LSQARE, RSQARE, LLPAREN, RLPAREN
    }

    public enum StateType {
        //开始、数字、标识符、赋值、注释、完成
        START, INNUM, INID, INASSIGN, INCOMMENT, DONE
    }

    public enum NodeKind{
        //声明，语句，表达式
        DeclaK, StmtK, ExpK
    }

    public enum DeclaKind{
        //变量声明， 函数，
        VarDeclK, FuncK, IntK, VoidK, ArryDeclK, ArryElemK
    }

    public enum StmtKind{
        // 比较， 表达式， 选择， 迭代， 返回，调用， 声明参数， 调用参数
        CompK, ExpK, SeleK, IteraK, RetuK, CallK, ParamK, ParamsK, ArgsK, Else, If
    }

    public enum ExpKind{
        // 操作符， 常量， 标识符， 赋值
        OpK, ConstK, IdK, AssignK
    }

}
