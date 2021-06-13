package com.exam;

import java.util.ArrayList;

public class Parse {

    static Const.TokenType token;
    static int step;

    /**
     * 语法错误，在打印语法树时先打印出错位置和出错字符串
     */
    static void syntaxError(String str) {
        try {
            Util.fileWriter.write("Syntax error at line " + Scan.lineNum + ": " + str + "\n");
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    /**
     * 匹配当前token，并获取下一个token
     */
    static void match(Const.TokenType expected) {
        if (token == expected ) {
            token = Scan.getToken();
        } else {
            syntaxError("unexpected token(" + expected.toString() + ") -> " + token.toString());
        }
    }

    /**
     * 函数由一组声明组成。declaration_list(void)函数使用递归向下分析方法直接调用declaration()函数，并返回树节点。依次获取每一个声明。
     */
    static TreeNode declarationList() {
        TreeNode t = declaration();
        TreeNode p = t;
        while (token != Const.TokenType.INT && token != Const.TokenType.VOID && token != Const.TokenType.ENDFILE) {
            syntaxError("DeclarationList Error");
            Scan.getToken();
            if (token == Const.TokenType.ENDFILE) {
                break;
            }
        }
        while (token == Const.TokenType.INT || token == Const.TokenType.VOID) {
            TreeNode q = declaration();
            p.sibling = q;
            p = q;
        }
        return t;
    }

    /**
     * C-语言的声明分为变量声明和函数声明。declaration()函数通过求First集合的方式先确定是变量定义还是函数定义，
     * 然后分别根据探测的结果生成变量声明节点（VarDeclK）或函数声明节点（FunK）。
     * 为语法树创建一个新的声明结点，匹配int或void后再匹配标识符(即参数)
     */
    static TreeNode declaration() {
        TreeNode t = newDeclaredNode(Const.DeclaKind.FuncK);
        if (token == Const.TokenType.INT) {
            t.child.add(newDeclaredNode(Const.DeclaKind.IntK));
            match(Const.TokenType.INT);
        } else if (token == Const.TokenType.VOID) {
            t.child.add(newDeclaredNode(Const.DeclaKind.VoidK));
            match(Const.TokenType.VOID);
        } else {
            syntaxError("Type matching error");
        }
        if (token == Const.TokenType.ID) {
            TreeNode q = newExpNode(Const.ExpKind.IdK);
            q.name = Scan.tokenString.toString();
            match(Const.TokenType.ID);
            if (token == Const.TokenType.LPAREN) {
                t.child.add(q);
                match(Const.TokenType.LPAREN);
                //参数
                t.child.add(params());
                match(Const.TokenType.RPAREN);
                //复合语句
                t.child.add(compoundStmt());
            } else if (token == Const.TokenType.LSQARE) {
                t.decla = Const.DeclaKind.VarDeclK;
                TreeNode m = newDeclaredNode(Const.DeclaKind.ArryDeclK);
                match(Const.TokenType.LSQARE);
                match(Const.TokenType.NUM);
                m.child.add(q);
                m.child.add(newExpNode(Const.ExpKind.ConstK));
                t.child.add(m);
                match(Const.TokenType.RSQARE);
                match(Const.TokenType.SEMI);
            } else if (token == Const.TokenType.SEMI) {
                t.child.add(q);
                match(Const.TokenType.SEMI);
            }
        } else {
            syntaxError("Declaration Error");
        }
        return t;
    }

    /**
     * 函数参数列表要么是void，要么是多个参数组成。params()函数先判断第一个是void还是int，如果是int说明是由param_list组成，
     * 则调用param_list(TreeNode k)函数递归向下分析；如果是void说明可能是void型的参数，也可能参数就是void，所以再求其Follow集合，
     * 如果集合求出来是右括号，则说明参数就是void，于是新建一个VoidK节点就行；如果集合求出来不是右括号则说明是void型的参数，
     * 然后再调用param_list(TreeNode k)函数递归向下分析，并将已经取出VoidK节点传递给param_list(TreeNode k)函数
     */
    static TreeNode params() {
        TreeNode t = newStmtNode(Const.StmtKind.ParamsK);
        TreeNode p;
        if (token == Const.TokenType.VOID) {
            p = newDeclaredNode(Const.DeclaKind.VoidK);
            match(Const.TokenType.VOID);
            if (token == Const.TokenType.RPAREN) {
                t.child.add(p);
            } else {
                t.child.add(paramList(p));
            }
        } else if (token == Const.TokenType.INT) {
            t.child.add(paramList(null));
        }else if(token == Const.TokenType.RPAREN){
            p = newDeclaredNode(Const.DeclaKind.VoidK);
            t.child.add(p);
        }else {
            syntaxError("Params Error");
        }
        return t;
    }

    /**
     * 参数列表由param序列组成，并由逗号隔开。param_list(TreeNode k)使用递归向下分析方法直接调用param(TreeNode k)，并返回树节点
     */
    static TreeNode paramList(TreeNode k) {
        TreeNode t = param(k);
        TreeNode p = t;
        while (token == Const.TokenType.COMMA) {
            TreeNode q;
            match(Const.TokenType.COMMA);
            q = param(null);
            p.sibling = q;
            p = q;
        }
        return t;
    }

    /**
     * 参数由int或void组成，最后可能有中括号表示数组。param(TreeNode k)函数根据探测先行Token是int还是void,
     * 来新建IntK或VoidK节点作为其第一个子节点，然后新建IdK节点作为其第二个子节点，最后探测Follow集合，是否是中括号，
     * 来确定是否再新建第三个子节点表示数组类型
     */
    static TreeNode param(TreeNode k) {
        TreeNode t = newStmtNode(Const.StmtKind.ParamK);
        TreeNode p = null;
        TreeNode q;
        if (k == null && token == Const.TokenType.INT) {
            p = newDeclaredNode(Const.DeclaKind.IntK);
            match(Const.TokenType.INT);
        } else if (k != null) {
            p = k;
        }
        if (p != null) {
            t.child.add(p);
            if (token == Const.TokenType.ID) {
                q = newExpNode(Const.ExpKind.IdK);
                q.name = Scan.tokenString.toString();
                t.child.add(q);
                match(Const.TokenType.ID);
            } else {
                syntaxError("Param Error");
            }
            if (token == Const.TokenType.LSQARE && t.child.get(1) != null) {
                match(Const.TokenType.LSQARE);
                t.child.add(newExpNode(Const.ExpKind.IdK));
                match(Const.TokenType.RSQARE);
            } else {
                return t;
            }
        }
        return t;
    }

    /**
     * 复合语句由用花括号围起来的一组声明和语句组成。compound_stmt()使用递归向下分析方法直接调用local_declaration()和statement_list()，
     * 并根据返回的树节点作为其第一个子节点和第二个子节点
     */
    static TreeNode compoundStmt() {
        TreeNode t = newStmtNode(Const.StmtKind.CompK);
        match(Const.TokenType.LLPAREN);
        t.child.add(localDeclaration());
        t.child.add(statementList());
        match(Const.TokenType.RLPAREN);
        return t;
    }

    /**
     * 局部变量声明要么是空，要么由许多变量声明序列组成。
     * local_declaration()函数通过判断先行的Token是否是int或void便可知道局部变量声明是否为空，
     * 如果不为空，则新建一个变量定义节点（VarDeclK）
     */
    static TreeNode localDeclaration() {
        TreeNode t = null;
        TreeNode q = null;
        TreeNode p;
        while (token == Const.TokenType.INT || token == Const.TokenType.VOID) {
            p = newDeclaredNode(Const.DeclaKind.VarDeclK);
            if (token == Const.TokenType.INT) {
                p.child.add(newDeclaredNode(Const.DeclaKind.IntK));
                match(Const.TokenType.INT);
            } else if (token == Const.TokenType.VOID) {
                p.child.add(newDeclaredNode(Const.DeclaKind.VoidK));
                match(Const.TokenType.VOID);
            }
            if (token == Const.TokenType.ID) {
                TreeNode tmp = newExpNode(Const.ExpKind.IdK);
                tmp.name = Scan.tokenString.toString();
                p.child.add(tmp);
                match(Const.TokenType.ID);
                if (token == Const.TokenType.LSQARE) {
                    p.child.add(newDeclaredNode(Const.DeclaKind.VarDeclK));
                    match(Const.TokenType.LSQARE);
                    match(Const.TokenType.RSQARE);
                }
                match(Const.TokenType.SEMI);
            } else {
                syntaxError("LocalDeclaration Error");
            }
            if (t == null) {
                t = q = p;
            } else {
                q.sibling = p;
                q = p;
            }
        }
        return t;
    }

    /**
     * 由语句列表由0到多个statement组成。statement_list()通过判断先行Token类型是否为statement的First集合确定后面是否是一个statement，
     * 如果是，则使用递归向下分析方法直接调用statement()函数
     */
    static TreeNode statementList() {
        TreeNode t = statement();
        TreeNode p = t;
        while (token == Const.TokenType.IF || token == Const.TokenType.LLPAREN ||
                token == Const.TokenType.ID || token == Const.TokenType.WHILE ||
                token == Const.TokenType.RETURN || token == Const.TokenType.SEMI ||
                token == Const.TokenType.LPAREN || token == Const.TokenType.NUM) {
            TreeNode q = statement();
            if (q != null) {
                if (t == null) {
                    t = p = q;
                } else {
                    p.sibling = q;
                    p = q;
                }
            }
        }
        return t;
    }

    /**
     * 选择、迭代、返回、复合、表达式语句
     */
    static TreeNode statement() {
        TreeNode t = null;
        switch (token) {
            case IF:
                t = selectionStmt();
                break;
            case WHILE:
                t = iterationStmt();
                break;
            case RETURN:
                t = returnStmt();
                break;
            case LLPAREN:
                t = compoundStmt();
                break;
            case ID:
            case SEMI:
            case LPAREN:
            case NUM:
                t = expressionStmt();
                break;
            default:
                syntaxError("Statement Error");
                token = Scan.getToken();
                break;
        }
        return t;
    }

    static TreeNode selectionStmt() {
        TreeNode t = newStmtNode(Const.StmtKind.SeleK);
        TreeNode q;
        match(Const.TokenType.IF);
        match(Const.TokenType.LPAREN);
        t.child.add(expression());
        match(Const.TokenType.RPAREN);
        t.child.add(statement());
        if (token == Const.TokenType.ELSE) {
            match(Const.TokenType.ELSE);
            q=newStmtNode(Const.StmtKind.Else);
            q.child.add(statement());
            t.child.add(q);
        }
        return t;
    }

    static TreeNode expressionStmt() {
        TreeNode t;
        if (token == Const.TokenType.SEMI) {
            match(Const.TokenType.SEMI);
            return null;
        } else {
            t = expression();
            match(Const.TokenType.SEMI);
        }
        return t;
    }

    static TreeNode iterationStmt() {
        TreeNode t = newStmtNode(Const.StmtKind.IteraK);
        match(Const.TokenType.WHILE);
        match(Const.TokenType.LPAREN);
        t.child.add(expression());
        match(Const.TokenType.RPAREN);
        t.child.add(statement());
        return t;
    }

    static TreeNode returnStmt() {
        TreeNode t = newStmtNode(Const.StmtKind.RetuK);
        match(Const.TokenType.RETURN);
        if (token == Const.TokenType.SEMI) {
            match(Const.TokenType.SEMI);
            return t;
        } else {
            t.child.add(expression());
        }
        match(Const.TokenType.SEMI);
        return t;
    }

    /**
     * 表达式
     */
    static TreeNode expression() {
        TreeNode t = var();
        if (t == null) {
            t = simpleExpression(null);
        } else {
            TreeNode p;
            if (token == Const.TokenType.EQ) {
                p = newExpNode(Const.ExpKind.AssignK);
                p.name = Scan.tokenString.toString();
                match(Const.TokenType.EQ);
                p.child.add(t);
                p.child.add(expression());
                return p;
            }else {
                t = simpleExpression(t);
            }
        }
        return t;
    }

    /**
     * simple_expression(TreeNode k)先调用additive_expression(TreeNode k)返回一个节点，
     * 然后再一直判断后面的Token是否为<=、<、>、>=、==、!=，如果是则新建OpK节点，然后令之前的节点为其第一个子节点，
     * 然后继续调用additive_expression(TreeNode k)返回一个节点，作为OpK节点的第二个节点
     */
    static TreeNode simpleExpression(TreeNode k) {
        TreeNode t = additiveExpression(k);
        if (token == Const.TokenType.ASSIGN || token == Const.TokenType.MORE || token == Const.TokenType.LESS) {
            TreeNode q = newExpNode(Const.ExpKind.OpK);
            q.op = token;
            q.name = Scan.tokenString.toString();
            q.child.add(t);
            t = q;
            match(token);
            t.child.add(additiveExpression(null));
        }
        return t;
    }

    /**
     * additive_expression(TreeNode k)函数先调用term(TreeNode k)函数返回一个节点，
     * 然后再一直判断后面的Token是否为+或-，如果是则新建OpK节点，然后令之前的节点为其第一个子节点，
     * 然后继续调用term(TreeNode k)函数返回一个节点，作为OpK节点的第二个节点
     */
    static TreeNode additiveExpression(TreeNode k) {
        TreeNode t = term(k);
        while (token == Const.TokenType.PLUS || token == Const.TokenType.MINUS) {
            TreeNode q = newExpNode(Const.ExpKind.OpK);
            q.op = token;
            q.name = Scan.tokenString.toString();
            q.child.add(t);
            match(token);
            q.child.add(term(null));
            t = q;
        }
        return t;
    }

    /**
     * term(TreeNode k)函数先调用factor(TreeNode k)函数返回一个节点，然后再一直判断后面的Token是否为*或/，
     * 如果是则新建OpK节点，然后令之前的节点为其第一个子节点，然后继续调用actor(TreeNode k)函数返回一个节点，作为OpK节点的第二个节点
     */
    static TreeNode term(TreeNode k) {
        TreeNode t = factor(k);
        while (token == Const.TokenType.TIMES || token == Const.TokenType.OVER) {
            TreeNode q = newExpNode(Const.ExpKind.OpK);
            q.op = token;
            q.name = Scan.tokenString.toString();
            q.child.add(t);
            match(token);
            q.child.add(factor(null));
            t = q;
        }
        return t;
    }

    /**
     * factor(TreeNode k)函数首先判断k是否为空，如果不为空，则k为上面传下来的已经解析出来的以ID开头的var，
     * 此时可能为call或var的情况，然后判断后面的Token是否为左括号，如果是则说明是call的情形，如果不是则为var的情形。
     * 如果k为空，再根据先行的Token类型判断是4中推导中的哪一种，然后直接调用相关的函数返回一个节点
     */
    static TreeNode factor(TreeNode k) {
        TreeNode t = null;
        if (k != null) {
            if (token == Const.TokenType.LPAREN) {
                t = call(k);
            } else {
                t = k;
            }
        } else {
            switch (token) {
                case LPAREN:
                    match(Const.TokenType.LPAREN);
                    t = expression();
                    match(Const.TokenType.RPAREN);
                    break;
                case ID:
                    k = var();
                    if (token == Const.TokenType.LPAREN) {
                        t = call(k);
                    } else {
                        t = k;
                    }
                    break;
                case NUM:
                    t = newExpNode(Const.ExpKind.ConstK);
                    if (token == Const.TokenType.NUM) {
                        t.val = Integer.parseInt(Scan.tokenString.toString());
                    }
                    match(Const.TokenType.NUM);
                    break;
                default:
                    syntaxError("Factor Error: " + token.toString());
                    token = Scan.getToken();
                    break;
            }
        }
        return t;
    }

    /**
     * var()先新建一个IdK节点，再通过判断先行Token类型是否为左中括号，如果是则新建一个数组节点Arry_ElemK，
     * 将IdK作为ArryElemK节点的第一个子节点，再调用函数expression()得到ArryElemK的第二个子节点，
     * 最后返回的节点是ArryElemK；如果先行Token类型不是左中括号，则将之前的IdK返回
     */
    static TreeNode var() {
        TreeNode t = null;
        TreeNode p;
        TreeNode q;
        if (token == Const.TokenType.ID) {
            p = newExpNode(Const.ExpKind.IdK);
            p.name = Scan.tokenString.toString();
            match(Const.TokenType.ID);

            if (token == Const.TokenType.LSQARE) {
                match(Const.TokenType.LSQARE);
                q = expression();
                match(Const.TokenType.RSQARE);
                t = newDeclaredNode(Const.DeclaKind.ArryElemK);
                t.child.add(p);
                t.child.add(q);
            }
//            //TODO 如果先行Token是左小括号，则
//            else if(token == Const.TokenType.LPAREN){
//                match(Const.TokenType.LPAREN);
//                p = newExpNode(Const.ExpKind.IdK);
//                p.name = Scan.tokenString.toString();
//                match(Const.TokenType.ID);
//            }
            else {
                t = p;
            }
        }
        return t;
    }

    /**
     * call(TreeNode k)函数新建一个call语句的节点CallK，如果k不为空，则将k设为CallK的第一个子节点，
     * 然后通过调用args()函数获得其第二个节点，最后返回CallK节点
     */
    static TreeNode call(TreeNode k) {
        TreeNode t = newStmtNode(Const.StmtKind.CallK);
        if (k != null) {
            t.child.add(k);
        }
        match(Const.TokenType.LPAREN);
        if (token == Const.TokenType.RPAREN) {
            match(Const.TokenType.RPAREN);
            return t;
        } else if (k != null) {
            t.child.add(args());
            match(Const.TokenType.RPAREN);
        }
        return t;
    }

    /**
     * args()函数首先判断后面的Token是否为右括号，如果是则说明是empty的情形，
     * 如果不是则为至少有一个expression的情形，然后调用expression()函数返回节点，然后一直判断后面的Token是否为逗号，
     * 如果是则说明后面还有一个expression，则再调用expression()函数，使各expression返回的节点为兄弟节点，
     * 然后再将第一个expression返回的节点作为函数调用语句参数节点ArgsK的子节点
     */
    static TreeNode args() {
        TreeNode t = newStmtNode(Const.StmtKind.ArgsK);
        TreeNode s = null;
        TreeNode p;
        if (token != Const.TokenType.RPAREN) {
            s = expression();
            p = s;
            while (token == Const.TokenType.COMMA) {
                TreeNode q;
                match(Const.TokenType.COMMA);
                q = expression();
                if (q != null) {
                    if (s == null) {
                        s = p = q;
                    } else {
                        p.sibling = q;
                        p = q;
                    }
                }
            }
        }
        if (s != null) {
            t.child.add(s);
        }
        return t;
    }

    static TreeNode newDeclaredNode(Const.DeclaKind kind) {
        TreeNode t = new TreeNode();
        t.nodeKind = Const.NodeKind.DeclaK;
        t.child = new ArrayList<>();
        t.decla = kind;
        t.lineNum = Scan.lineNum;
        return t;
    }

    static TreeNode newExpNode(Const.ExpKind kind) {
        TreeNode t = new TreeNode();
        t.nodeKind = Const.NodeKind.ExpK;
        t.child = new ArrayList<>();
        t.exp = kind;
        t.lineNum = Scan.lineNum;
        return t;
    }

    static TreeNode newStmtNode(Const.StmtKind kind) {
        TreeNode t = new TreeNode();
        t.nodeKind = Const.NodeKind.StmtK;
        t.child = new ArrayList<>();
        t.stmt = kind;
        t.lineNum = Scan.lineNum;
        return t;
    }

    /**
     * C-语言编写的程序由函数组成。parse()函数使用递归向下分析方法直接调用declaration_list()函数，并返回树节点
     */
    static TreeNode parse() {
        step = 0;
        TreeNode root;
        token = Scan.getToken();
        root = declarationList();
        if (token != Const.TokenType.ENDFILE) {
            syntaxError("Code ends before file\n");
        }
        return root;
    }
}
