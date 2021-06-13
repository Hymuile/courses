package com.example;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * 词法分析器
 *
 * @author 24206
 */
public class LexicalAnalyzer {

    private static BufferedReader br;
    private static ArrayList<Token> tokens = new ArrayList<>();

    /**
     * 判断是否为关键(保留)字
     */
    public static boolean isReservedWord(String str) {
        for (ReservedWord rw : ReservedWord.values()) {
            if (rw.getReservedWord().equals(str)) {
                return true;
            }
        }
        return false;
    }

    /**
     * 判断是否为专用字符
     */
    public static boolean isDelimiter(char str) {
        for (Delimiter delimiter : Delimiter.values()) {
            if (delimiter.getDelimiter() == str) {
                return true;
            }
        }
        return false;
    }

    /**
     * 判断是否为数字
     */
    public static boolean isNum(char ch) {
        return ch >= '0' && ch <= '9';
    }

    /**
     * 判断是否为字母
     */
    public static boolean isLetter(char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    /**
     * 读文件
     *
     * @param fileSrc 读取文件名
     * @return 文件内容
     */
    public static StringBuilder readFile(String fileSrc) {
        StringBuilder sb = new StringBuilder();
        try {
            FileReader fileReader = new FileReader(fileSrc);
            BufferedReader br = new BufferedReader(fileReader);
            String temp = null;
            while ((temp = br.readLine()) != null) {
                sb.append(temp);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return sb;
    }

    /**
     * 词法分析
     *
     * @throws Exception 异常
     */
    public static List<Token> lexAnalysis(String srcPath) throws Exception {
        br = new BufferedReader(new FileReader(srcPath));
        StringBuilder lines = readFile(srcPath);
        int i = 0;
        while (i < lines.length()) {
            char ch = lines.charAt(i);
            //字母
            if (isLetter(ch)) {
                StringBuilder sb = new StringBuilder();
                sb.append(ch);
                if (i + 1 < lines.length()) {
                    ch = lines.charAt(++i);
                }
                while (isLetter(ch) || isNum(ch)) {
                    sb.append(ch);
                    i++;
                    //如果读到行末尾则结束，否则继续读
                    if (i >= lines.length() - 1) {
                        break;
                    } else {
                        ch = lines.charAt(i);
                    }
                }
                if (isReservedWord(sb.toString())) {
                    tokens.add(new Token("reservedWord", sb.toString()));
                } else {
                    tokens.add(new Token("ID", sb.toString()));
                }
            }

            //专用字符
            else if (isDelimiter(ch)) {
                StringBuilder sb = new StringBuilder();
                //如果是单分界符，直接写入
                if (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '[' ||
                        ch == ']' || ch == '{' || ch == '}' || ch == '+' || ch == '-' || ch == '*') {
                    tokens.add(new Token("delimiter", String.valueOf(ch)));
                    i++;
                }
                //判断是否为双分界符
                else if (ch == '>' || ch == '<' || ch == '=') {
                    sb.append(ch);
                    char nextCh = lines.charAt(++i);
                    if (nextCh == '=') {
                        sb.append(nextCh);
                        tokens.add(new Token("delimiter", sb.toString()));
                        i++;
                    } else {
                        tokens.add(new Token("delimiter",String.valueOf(ch)));
                    }
                }
                //判断注释与除法
                else if (ch == '/') {
                    sb.append(ch);
                    ch = lines.charAt(++i);
                    //为/*注释
                    if (ch == '*') {
                        while (true) {
                            ch = lines.charAt(++i);
                            // 为多行注释结束
                            if (ch == '*') {
                                ch = lines.charAt(++i);
                                if (ch == '/') {
                                    ++i;
                                    break;
                                }
                            }
                        }
                    } else {
                        tokens.add(new Token("delimiter", sb.toString()));
                    }
                }
            }

            //数字
            else if (isNum(ch)) {
                StringBuilder sb = new StringBuilder();
                sb.append(ch);
                ch = lines.charAt(++i);
                if (isNum(ch)) {
                    while (isNum(ch)) {
                        sb.append(ch);
                        ch = lines.charAt(++i);
                    }
                }
                tokens.add(new Token("number", sb.toString()));
                if (isLetter(ch)) {
                    System.out.println("error:非法字符");
                }
            }

            //其他字符
            else {
                i++;
            }
        }
        br.close();
        return tokens;
    }

}