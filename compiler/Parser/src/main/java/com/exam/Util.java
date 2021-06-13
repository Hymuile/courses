package com.exam;

import java.io.*;

public class Util {

    static FileInputStream fileInputStream;
    static BufferedReader bufferedReader;
    static FileWriter fileWriter;

    static void getFile(String path) throws Exception {
        Const.sourceFile = new File(path);
        fileInputStream = new FileInputStream(Const.sourceFile);
        bufferedReader = new BufferedReader(new InputStreamReader(fileInputStream));
    }

    static void makeFile(String path) throws Exception {
        Const.targetFile = new File(path);
        fileWriter = new FileWriter(Const.targetFile, false);
    }

    static void printSpace(int n) throws Exception {
        for (int i = 0; i < n; i++) {
            fileWriter.write("\t");
        }
    }

    static void printKind(String str) throws Exception {
        fileWriter.write(str + "\n");
    }

    static void printTree(TreeNode t) throws Exception {
        while (t != null) {
            printSpace(Parse.step);
            switch (t.nodeKind) {
                case DeclaK:
                    switch (t.decla) {
                        case VoidK:
                            printKind("VoidK");
                            break;
                        case IntK:
                            printKind("IntK");
                            break;
                        case FuncK:
                            printKind("FuncK");
                            break;
                        case VarDeclK:
                            printKind("Var_DeclK");
                            break;
                        default:
                            break;
                    }
                    break;
                case StmtK:
                    switch (t.stmt) {
                        case ExpK:
                            printKind("ExpK");
                            break;
                        case CompK:
                            printKind("CompK");
                            break;
                        case RetuK:
                            printKind("Return");
                            break;
                        case Else:
                            printKind("Else");
                            break;
                        case SeleK:
                            printKind("If");
                            break;
                        case IteraK:
                            printKind("While");
                            break;
                        case ParamsK:
                            printKind("ParamsK");
                            break;
                        case ParamK:
                            printKind("ParamK");
                            break;
                        case ArgsK:
                            printKind("ArgsK");
                            break;
                        case CallK:
                            printKind("CallK");
                            break;
                        default:
                            break;
                    }
                    break;
                case ExpK:
                    switch (t.exp) {
                        case IdK:
                            printKind("IdK: " + t.name);
                            break;
                        case OpK:
                            printKind("Op: " + t.name);
                            break;
                        case ConstK:
                            printKind("ConstK: " + t.val);
                            break;
                        case AssignK:
                            printKind("AssignK");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }

            Parse.step++;
            for (TreeNode item : t.child) {
                printTree(item);
            }
            Parse.step--;
            t = t.sibling;
        }
    }
}
