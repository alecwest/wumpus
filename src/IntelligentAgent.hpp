
/* Generated from GenMyModel */

#ifndef DEF_INTELLIGENTAGENT
#define DEF_INTELLIGENTAGENT

#include "Agent.hpp"
#include "KnowledgeBase.hpp"

class IntelligentAgent : public Agent
{
private:
	KnowledgeBase kb;
public:
	void makeMove();
};


#endif
