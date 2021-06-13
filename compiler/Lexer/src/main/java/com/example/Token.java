package com.example;

public class Token {
    public String type;
    public String value;
    public Token(String type,String value){
        this.type = type;
        this.value = value;
    }

    @Override
    public String toString() {
        return "<"+type+" , "+value+">";
    }
}
