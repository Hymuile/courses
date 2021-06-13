package com.example;

import java.util.ArrayList;

public class TestLex {
    public static void main(String[] args) {
        LexicalAnalyzer lexicalAnalysis=new LexicalAnalyzer();
        try {
            ArrayList<Token> tokens = (ArrayList<Token>) lexicalAnalysis.lexAnalysis("src/main/resources/input.txt");
            for(Token token : tokens){
                System.out.println(token);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
