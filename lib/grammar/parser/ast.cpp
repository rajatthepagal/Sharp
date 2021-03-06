//
// Created by bknun on 1/7/2017.
//
#include "ast.h"


ast_types ast::gettype()
{
    return type;
}

ast* ast::getparent()
{
    return parent;
}

long ast::getsubastcount()
{
    return numAsts;
}

ast* ast::getsubast(long at)
{
    if(numAsts == 0) return NULL;
    return &(*std::next(sub_asts->begin(), at));
}

long ast::getentitycount()
{
    return numEntities;
}

token_entity ast::getentity(long at)
{
    if(numEntities == 0) return token_entity();
    return *std::next(entities->begin(), at);
}

void ast::add_entity(token_entity entity)
{
    numEntities++;
    entities->push_back(entity);
}

void ast::add_ast(ast _ast)
{
    numAsts++;
    sub_asts->push_back(_ast);
}

void ast::free() {
    this->type = ast_none;
    this->parent = NULL;
    this->entities->clear();
    numAsts = 0;
    numEntities = 0;
    std::free(this->entities); this->entities = NULL;

    ast* pAst;
    for(int64_t i = 0; i < this->sub_asts->size(); i++)
    {
        pAst = &(*std::next(this->sub_asts->begin(),
                     i));
        pAst->free();
    }

    this->sub_asts->clear();
    std::free(this->sub_asts); this->sub_asts = NULL;
}

void ast::freesubs() {
    ast* pAst;
    for(int64_t i = 0; i < this->sub_asts->size(); i++)
    {
        pAst = &(*std::next(this->sub_asts->begin(),
                            i));
        pAst->free();
    }

    numAsts = 0;
    this->sub_asts->clear();
}

void ast::freelastsub() {
    ast* pAst = &(*std::next(this->sub_asts->begin(),
                             this->sub_asts->size()-1));
    pAst->free();
    numAsts--;
    this->sub_asts->pop_back();
}

void ast::freeentities() {
    numEntities = 0;
    this->entities->clear();
}

void ast::freelastentity() {
    numEntities--;
    this->entities->pop_back();
}

bool ast::hassubast(ast_types at) {
    for(ast &pAst : *sub_asts) {
        if(pAst.gettype() == at)
            return true;
    }
    return false;
}

bool ast::hasentity(token_type t) {

    for(token_entity &e : *entities) {
        if(e.gettokentype() == t)
            return true;
    }
    return false;
}

