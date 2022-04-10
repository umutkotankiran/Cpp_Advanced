/*

Geçen ders facade ı inceliyorduk. 
Facade subsystem karmaşıklığını gizleyip bize basit bir arayüz sunuyordu.
Buradaki örneği al.Adı 

FACADE al

FACADE2 al

--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

COMPOSITE KALIBI
----------------
Nesneleri ağaç yapıları şeklinde ifade etmemizi ve bu yapılardan her birini tek bir 
nesne olarak kullanabilmemizi sağlar.

Uygulamamızın ana modeli bir ağaç yapısı olarak ifade edilebiliyor ise 
composite tasarım kalıbını kullanabiliriz.

Burada çok fazla açıklama var şekil ile beraber olduğunda anlam ifade ediyorlar.
11:00


ÖR:

main.cpp
--------

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

class Component
{
protected:
	Component *m_parent;

public:
	virtual ~Component() = default;
	void set_parent(Component* parent)
	{
		m_parent = parent;
	}

	Component* get_parent()const
	{
		return m_parent;
	}

	virtual void add(Component*) = 0;
	virtual void remove(Component*)  = 0;
	

	virtual bool is_composite()const
	{
		return false;
	}
	virtual std::string operation()const = 0;
};


class Leaf : public Component
{
public:
	virtual void add(Component *)override
	{
		//özellikle boş yada exception throw edilebilirdi.
	}
	
	virtual void remove(Component *)override
	{
		//özellikle boş yada exception throw edilebilirdi.
	}

	std::string operation()const override
	{
		return "Leaf";
	}
};

class Composite : public Component
{
protected:
	std::list<Component*> m_children_list;
public:
	void add(Component* pcom)override
	{
		m_children_list.push_back(pcom);
		pcom->set_parent(this);
	}

	void remove(Component* pcomp)override
	{
		m_children_list.remove(pcomp);
		pcomp->set_parent(nullptr);
	}

	bool is_composite()const override
	{
		return true;
	}

	std::string operation()const override
	{
		std::string result;
		for(const Component* c : m_children_list)
		{
			if(c == m_children_list.back())
			{
				result += c->operation();
			}
			else
			{
				result += c->operation() + "+";
			}
		}
		return "Branch(" + result + ")";
	}
};

void ClientCode(Component* pcomp)
{
	//...
	std::cout << "RESULT: " << pcomp->operation();
	//...
}

void ClientCode2(Component* component1, Component* component2)
{
	 //..
	 if(component1->is_composite())
	 {
		component1->add(component2);
	 }
	 std::cout << "RESULT: " << component1->operation();
	 //..
}

int main()
{
	//ADIM1
	Component *simple = new Leaf;
	std::cout << "Client : I have got a simple component\n";
	ClientCode(simple);
	std::cout << "\n\n";
	delete simple;

	//---------------------------------------------------------------------

	//ADIM2
	Component *simple = new Leaf;
	std::cout << "Client : I have got a simple component\n";
	ClientCode(simple);

	Component *tree = new Composite;
	Component *branch1 = new Composite;

	Component* leaf_1 = new Leaf;
	Component* leaf_2 = new Leaf;
	Component* leaf_3 = new Leaf;

	branch->add(leaf_1);
	branch->add(leaf_2);

	Component* branch2 = new Composize;
	branch2->add(leaf_3);

	tree->add(branch1);
	tree->add(branch2);
	std::cout << "Client : Now I have got a composite tree:\n";
	ClientCode(tree);

	delete simple;
	std::cout << "\n\n";
}

--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

ÖR:

component.h
-----------
#pragma once
#include <vector>

class Component
{
public:
	virtual ~Component() = default;
	virtual void operation() = 0;
	virtual void add_component(Component *) = 0;
	virtual void remove_component(Component *) = 0;
};


component.cpp
-------------
#include "component.h"
#include <iostream>
#include <algorithm>

void Component::operation()
{
	std::cout << "composite operation\n";
	for(auto pcomp : m_comp_vec)
	{
		pcomp->operation();
	}
}

void Component::add_component(Component* pcomp)
{
	m_comp_vec.push_back(pcomp);
}

void Component::remove_component(Component* pcomp)
{
	m_comp_vec.erase(remove(m_comp_vec.begin(),m_comp_vec.end(),pcomp), m_comp_vec.end()); // erase remove idiyomu
	
	// C++20 ile erase remova a gerek yok. Aşağıdaki gibi yapılabilir.
	// erase(m_comp_vec, pcomp); // adl de var ayrıca
}



leaf.h
------
#pragma once
#include "component.h"
#include <iostream>

class Leaf : public Component
{
public:
	virtual void operation()override
	{
		std::cout << "Leaf::operation()\n";
	}
	virtual void add_component(Component *)override
	{
		// özellikle boş bırakıldı. Böyle olmayadabilir
	}
	virtual void remove_component(Component *) overrid e
	{

	}
};


composite.h
-----------
#pragma once
#include "component.h"
#include <vector>


class Composite : public Component
{
public:
	virtual void operation()override;
	virtual void add_component(Component *)override;
	virtual void remove_component(Component *)override;
private:
	std::vector<Component*> m_comp_vec;
};


composite.cpp
-------------
#include "component.h"
#include <iostream>
#include <algorithm>

void Composite::operation()
{
	std::cout << "composite operation\n";
	for(auto pcomp : m_comp_vec)
	{
		pcomp->operation();
	}
}

void Composite::add_component(Component* pcomp)
{
	m_comp_vec.push_back(pcomp);
}

void Composite ::remove_component(Component* pcomp)
{
	m_comp_vec.erase(remove(m_comp_vec.begin(),m_comp_vec.end(),pcomp), m_comp_vec.end()); // erase remove idiyomu

	// C++20 ile erase remova a gerek yok. Aşağıdaki gibi yapılabilir.
	// erase(m_comp_vec, pcomp); // adl de var ayrıca
}



main.cpp
--------

#include "leaf.h"
#include "component.h"
#include "composite.h"

void func(Component *pcomp)
{
	pcomp->operation();
}

int main()
{
	Leaf lf_1, lf_2, lf_3;
	Composite root;

	root.add_component(&lf_1);
	root.add_component(&lf_2);

	Composite branch;
	branch.add_component(&lf_3);

	root.add_component(&branch);

	func(&root);

}


MOLA

================================================================================================
================================================================================================
================================================================================================
================================================================================================

COMMAND KALIBI
--------------
Bir isteği bu isteğin gerçekleştirilmesine ilişkin tüm bilgileri içeren bağımsız b ir nesneye döndürür.
Burada da powerpoint konu anlatımından ilerlenebilir.
Şekillerle beraber açıklamalar var.

Command burada arakatman gibi çalışıyor.

ÖR:

#include <string>
#include <iostream>

class Command{
public:
	virtual ~Command() { }
	virtual void execute()const = 0;
};


class SimpleCommand : public Command
{
private:
	
public:
	explicit SimpleCommand(std::string pay_load) : pay_load_(pay_load){ }

	void execute() const override
	{
		std::cout << "SimpleCommand: See, I can do simple things like printing (" << pay_load_ << ")\n"; 
	}

private:
	std::string pay_load_;
};


class Receiver
{
public:
	void DoSomething(const std::string& a)
	{
		std::cout << "Receiver: Working on (" << a << ".)\n";
	}
	
	void DoSomethingElse(const std::string& b)
	{
		std::cout << "Receiver: Also Working on (" << b << ".)\n";
	}
};


class ComplexCommand : public Command{
private:
	Receiver *receiver_;

	std::string a_;
	std::string b_;

public:
	ComplexCommand(Receiver* receiver, std::string a, std::string b) : receiver_(receiver), a_(a), b_(b)
	{
	}

	void execute()const override
	{
		std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
		receiver_->DoSomething(a_);
		receiver_->DoSomethingElse(b_);
	}
};


class Invoker{

private:
	Command *on_start_;

	Command *on_finish_;

public:
	~Invoker()
	{
		delete on_start_;
		delete on_finish_;
	}

	void SetOnStart(Command* command)
	{
		on_start_ = command;
	}

	void SetOnFinist(Command* command)
	{
		on_finish_ = command;
	}

	void DoSomethingImportant()
	{
		std::cout << "Invoker: Doest anybody want something done before i begin?\n";
		if(on_start_)
		{
			on_start_->execute();
		}
		
		std::cout << "Invoker: ... doing something really important...\n";
		std::cout << "Invoker: Does anybody want something done after I finish?\n";

		if(on_finish_)
		{
			on_finish_->execute();
		}
	}
};

int main()
{
	Invoker* invoker = new Invoker;	// buton gibi bu
	invoker->SetOnStart(new SimpleCommand("Say Hi!"));

	Receiver rx;
	invoker -> SetOnFinish(new ComplexCommand(&rx, "Send email", "Save Report"));
	invoker -> DoSomethingImportant();

	delete invoker;
}

*/
