#include <iostream>
#include "error.h"
#include "gen.h"
using std::cout;
using std::endl;

extern Lexer * scanner;

Expression *Lvalue(Expression *n)
{
    if (n->node_type == NodeType::IDENTIFIER)
    {
        return n;
    }
    else if (n->node_type == NodeType::ACCESS)
    {
        Access * a = (Access*) n;
        return new Access(a->type, a->token, a->id, Rvalue(a->expr));
    }
    else
    {
        stringstream ss;
        ss << "Expressão \'" << n->ToString() << "\' não possui valor-l";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}

Expression *Rvalue(Expression *n)
{
    if (n->node_type == NodeType::IDENTIFIER || n->node_type == NodeType::CONSTANT)
    {
        return n;
    }
    else if (n->node_type == NodeType::ARI)
    {   
        Arithmetic * ari = (Arithmetic*) n;
        Temp * t = new Temp(ari->type);
        Expression * e1 = Rvalue(ari->expr1);
        Expression * e2 = Rvalue(ari->expr2);
        cout << t->ToString() << " = " 
             << e1->ToString() << " " 
             << ari->ToString() << " " 
             << e2->ToString() << endl;
        return t;
    }
    else if (n->node_type == NodeType::REL)
    {
        Relational * rel = (Relational*) n;
        Temp * t = new Temp(rel->type);
        cout << t->ToString() << " = " 
             << Rvalue(rel->expr1)->ToString() 
             << " " << rel->ToString() << " "
             << Rvalue(rel->expr2)->ToString() 
             << endl;
        return t;
    }
    else if (n->node_type == NodeType::ACCESS)
    {
        Access * access = (Access*) n;
        Temp * temp = new Temp(access->type);
        Expression * right = Lvalue(n);
        cout << temp->ToString() << " = " 
             << right->ToString() 
             << endl;
        return temp;
    }
    else if (n->node_type == NodeType::ASSIGN)
    {
        Access * acc = (Access*) Lvalue(n);
        Expression * left = Lvalue(acc->id);
        Expression * right = Rvalue(acc->expr);
        cout << left->ToString()  
             << " = " 
             << right->ToString() 
             << endl;
        return right;
    }
    else
    {
        stringstream ss;
        ss << "Expressão \'" << n->ToString() << "\' não possui valor-r";
        throw SyntaxError{scanner->Lineno(), ss.str()};
    }
}