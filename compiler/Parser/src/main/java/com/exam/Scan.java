package com.exam;

/**
 * @author 24206
 */
public class Scan {

    static char[] lineBuf;
    static int linePos = 0;
    static int lineNum = 0;
    static boolean flagEOF = false;
    static String nextLine;
    static StringBuilder tokenString;

    static Character getNextChar() {
        if (lineBuf == null || (linePos >= lineBuf.length)) {
            try {
                if ((nextLine = Util.bufferedReader.readLine()) != null) {
                    lineNum++;
                    lineBuf = nextLine.toCharArray();
                    linePos = 0;
                    return lineBuf.length == 0 ? '\n' : lineBuf[linePos++];
                } else {
                    flagEOF = true;
                    Util.fileInputStream.close();
                    Util.bufferedReader.close();
                }
            } catch (Exception e) {
                System.out.println(e);
            }
            return Const.EOF;
        } else{
            return lineBuf[linePos++];
        }
    }

    static void unGetNextChar() {
        if (!flagEOF) {
            linePos--;
        }
    }

    static Const.TokenType getToken() {
        int tokenStringIndex = 0;
        tokenString = new StringBuilder();
        Const.TokenType currentToken = null;
        Const.StateType state = Const.StateType.START;
        boolean isSave;
        while (state != Const.StateType.DONE) {
            Character ch = getNextChar();
            isSave = true;
            switch (state) {
                case START:
                    if (Character.isDigit(ch)) {
                        state = Const.StateType.INNUM;
                    } else if (Character.isAlphabetic(ch)) {
                        state = Const.StateType.INID;
                    } else if (ch == ' ' || ch == '\t' || ch == '\n') {
                        isSave = false;
                    } else if (ch == '<' || ch == '>' || ch == '=' || ch == '!' || ch == '/'){
                        state = Const.StateType.INASSIGN;
                    }else {
                        state = Const.StateType.DONE;
                        switch (ch) {
                            case Const.EOF:
                                isSave = false;
                                currentToken = Const.TokenType.ENDFILE;
                                break;
                            case '+':
                                currentToken = Const.TokenType.PLUS;
                                break;
                            case '-':
                                currentToken = Const.TokenType.MINUS;
                                break;
                            case '*':
                                currentToken = Const.TokenType.TIMES;
                                break;
                            case '(':
                                currentToken = Const.TokenType.LPAREN;
                                break;
                            case ')':
                                currentToken = Const.TokenType.RPAREN;
                                break;
                            case ';':
                                currentToken = Const.TokenType.SEMI;
                                break;
                            case ',':
                                currentToken = Const.TokenType.COMMA;
                                break;
                            case '[':
                                currentToken = Const.TokenType.LSQARE;
                                break;
                            case ']':
                                currentToken = Const.TokenType.RSQARE;
                                break;
                            case '{':
                                currentToken = Const.TokenType.LLPAREN;
                                break;
                            case '}':
                                currentToken = Const.TokenType.RLPAREN;
                                break;
                            default:
                                currentToken = Const.TokenType.ERROR;
                                break;
                        }
                    }
                    break;
                case INNUM:
                    if (!Character.isDigit(ch)) {
                        unGetNextChar();
                        isSave = false;
                        state = Const.StateType.DONE;
                        currentToken = Const.TokenType.NUM;
                    }
                    break;
                case INID:
                    if (!Character.isAlphabetic(ch)) {
                        unGetNextChar();
                        isSave = false;
                        state = Const.StateType.DONE;
                        currentToken = Const.TokenType.ID;
                    }
                    break;
                case INASSIGN:
                    if (tokenString.charAt(tokenStringIndex - 1) == '/') {
                        state = Const.StateType.DONE;
                        if (ch == '*') {
                            state = Const.StateType.INCOMMENT;
                        }else {
                            unGetNextChar();
                            isSave = false;
                            currentToken = Const.TokenType.OVER;
                        }
                    } else if (tokenString.charAt(tokenStringIndex - 1) == '<') {
                        state = Const.StateType.DONE;
                        if (ch == '=') {
                            currentToken = Const.TokenType.ASSIGN;
                        }else {
                            unGetNextChar();
                            isSave = false;
                            currentToken = Const.TokenType.LESS;
                        }
                    } else if (tokenString.charAt(tokenStringIndex - 1) == '>') {
                        state = Const.StateType.DONE;
                        if (ch == '=') {
                            currentToken = Const.TokenType.ASSIGN;
                        }else {
                            unGetNextChar();
                            isSave = false;
                            currentToken = Const.TokenType.MORE;
                        }
                    } else if (tokenString.charAt(tokenStringIndex - 1) == '=') {
                        state = Const.StateType.DONE;
                        if (ch == '=') {
                            currentToken = Const.TokenType.ASSIGN;
                        }else {
                            unGetNextChar();
                            isSave = false;
                            currentToken = Const.TokenType.EQ;
                        }
                    } else if (tokenString.charAt(tokenStringIndex - 1) == '!') {
                        state = Const.StateType.DONE;
                        if (ch == '=') {
                            currentToken = Const.TokenType.ASSIGN;
                        }else {
                            unGetNextChar();
                            isSave = false;
                            currentToken = Const.TokenType.ERROR;
                        }
                    }
                    break;
                case INCOMMENT:
                    if (ch == Const.EOF) {
                        state = Const.StateType.DONE;
                        currentToken = Const.TokenType.ENDFILE;
                    }
                    if (tokenString.charAt(tokenStringIndex - 1) == '*' && ch == '/') {
                        isSave = false;
                        tokenString = new StringBuilder();
                        state = Const.StateType.START;
                    }
                    break;
                case DONE:
                    break;
                default:
                    try {
                        Util.fileWriter.write("Scanner Bug: state = " + state);
                        state = Const.StateType.DONE;
                        currentToken = Const.TokenType.ERROR;
                        break;
                    } catch (Exception e) {
                        System.out.println(e);
                    }
            }
            if (isSave) {
                tokenString.append(ch);
                tokenStringIndex++;
            }
            if (state == Const.StateType.DONE) {
                // IF, ELSE, INT, RETURN, VOID, WHILE
                if ("if".equals(tokenString.toString())) {
                    currentToken = Const.TokenType.IF;
                }else if ("else".equals(tokenString.toString())) {
                    currentToken = Const.TokenType.ELSE;
                }else if ("int".equals(tokenString.toString())) {
                    currentToken = Const.TokenType.INT;
                }else if ("return".equals(tokenString.toString())) {
                    currentToken = Const.TokenType.RETURN;
                }else if ("void".equals(tokenString.toString())) {
                    currentToken = Const.TokenType.VOID;
                }else if ("while".equals(tokenString.toString())) {
                    currentToken = Const.TokenType.WHILE;
                }
            }
        }
        return currentToken;
    }
}
