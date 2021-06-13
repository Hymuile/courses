package com.exam;

import java.util.List;

public class TreeNode {

    List<TreeNode> child;
    TreeNode sibling;
    int lineNum;
    Const.NodeKind nodeKind;
    Const.DeclaKind decla;
    Const.StmtKind stmt;
    Const.ExpKind exp;
    Const.TokenType op;
    int val;
    String name;
}
