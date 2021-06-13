package com.exam;

public class TestParse {
    public static void main(String[] args) throws Exception{
        String sourcePath = "src/main/resources/input.txt";
        String targetPath = "src/main/resources/output.txt";
        Util.getFile(sourcePath);
        Util.makeFile(targetPath);

        TreeNode root = Parse.parse();
        Util.printKind("Syntax Tree:");
        Parse.step++;
        Util.printTree(root);
        Util.fileWriter.close();
    }
}
