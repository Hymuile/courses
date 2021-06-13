package com.example;

public enum ReservedWord {
    IF("if"), ELSE("else"), INT("int"),RETURN("return"),VOID("void"),WHILE("while");

    private final String reservedWord;

    ReservedWord(String reservedWord){
        this.reservedWord = reservedWord;
    }

    public String getReservedWord() {
        return reservedWord;
    }
}

enum Delimiter{
    PARENTHESIS_L('('), PARENTHESIS_R(')'), MUL('*'), SLASH('/'), ADD('+'), SUB('-'),SEMICOLON(';'), COMMA(','),
    LBRACKET('['),RBRACKET(']'), LBRACE('{'), RBRACE('}'), LT('<'),ASSIGN('='), GT('>'), NOT('!'), ;

    public char getDelimiter() {
        return delimiter;
    }

    private final char delimiter;

    Delimiter(char delimiter){
        this.delimiter=delimiter;
    }
}