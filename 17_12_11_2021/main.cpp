/*

37DK DİZİ VE DİZİ POİNTER TEKRARI YAPILDI BİRAZ TEMEL C DE GÖRÜLDÜ DETAYLI
YAZMIYORUM

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

ORDER OF EVALUATION
-------------------
Sequence point kavramı(yanetki noktası) C den geliyor.C++ ta bazı kural farklılıkları var.C++17 de bazı değişiklikler yapıldı.

; , && || if ? : for içi if içi while içi func call içi

x = x++; Mesela bu C de UB

C++17 ile C de undefined behavior olan durumların bazıları well-defined behavior.
Func(expr1,expr2,expr3) hangi exprnin önce çalışacağı derleyiciye bağlı.

-----------------------------------------------------------------------------------------------------------------------------------------

ÖR:

class Myclass{
public:
	Myclass& foo(int)
	{
		return *this;
	}
	
	Myclass& foo(double)
	{
		return *this;
	}
};

int get_i()
{
	std::cout << "get_i\n";
	return 1;
}

double get_d()
{
	std::cout << "get_d\n";
	return 1.5;
}

int main()
{
	Myclass mx;

	mx.foo(get_i()).foo(get_d()); //fluent api - chaining.
	// Burada get_i nin get_d den önce çağrılma garantisi var mı?
	// C++17 den önce bu garanti yoktu.Bu programcının başını belaya sokuyordu
	// C++17 den sonra ise get_i, get_d den önce çağrılır.

	--------------------------------------------------------------------------------------------------

	std::cout << get_i() << get_d() ; // C++17 den önce hangisinin daha önce çağrılacağız belli değil.
					// C++17 den sonra get_i den sonra get_d çağrılır.
}

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

BELIRLI OPERATORLER İSTER OPERANDLARI PRİMİTİVE TÜRDEN OLSUN, İSTER OPERATOR OVERLOADİNG İLE ÇAĞRILAN FUNCLAR OLSUN,
BELİRLİ OPERATÖRLER İÇİN İŞLEMİN DAHA ÖNCE YAPILMA GARANTİSİ VAR.BUNA SEQUENCE BEFORE DENİYOR.

Sol operand full evaluated olacak sonra sağ operand yapılacak.

Not: Operator önceliği ile bu karıştırılmamalı.Bu operatorlerin operandı olan ifadelerin seçimi ile ilgili.
İşlemin önce yapılması ise sequence before ile ilgili.Mülakatlarda çok soruluyor.

int x = f1() + 5 * f2(); //Bir ifadenin bir sequence point yoksa subs expr leri için hangisinin daha önce işleme sokulacağı
konusunda garanti yok.Aynı derleyici farklı contexlerde farklı kod üretebilir.

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

BELİRLİ OPERATORLER İÇİN İŞLEMİN ÖNCE YAPILMASI YAN ETKİLERİN TAMAMLANMASINDAN SONRA DİĞER İŞLEMLERİN
YAPILMA GARANTİSİ VAR.

Hangi operatorlerde garanti var.
nokta(.)     a.b.c.d
.*
->*
<<
>>
[]
=
işlem=
() func calldaki sıra

Operatorlerin tam listesine bakalım.
Burada 14. yada 12. maddelerde önemli bunları bi okumak gerekir.
https://en.cppreference.com/w/cpp/language/eval_order

CPPREFERENCE KURALLAR
---------------------
1) Each value computation and side effect of a full expression is sequenced before each value computation and side effect of the next full expression,
   where a full expression is
 - an unevaluated operand
 - a constant expression
 - an immediate invocation(since C++20)
 - an entire initializer, including any comma-separated constituent expressions
 - the destructor call generated at the end of the lifetime of a non-temporary object
 - an expression that is not part of another full-expression (such as the entire expression statement, controlling expression of a for/while loop, conditional expression of if/switch, the expression in a return statement, etc),
   including implicit conversions applied to the result of the expression, destructor calls to the temporaries, default member initializers (when initializing aggregates), and every other language construct that involves a function call.
2) The value computations (but not the side-effects) of the operands to any operator are sequenced before the value computation of the result of the operator (but not its side-effects).
3) When calling a function (whether or not the function is inline, and whether or not explicit function call syntax is used), every value computation and side effect associated with any argument expression, or with the postfix expression designating the called function, is sequenced before execution of every expression or statement in the body of the called function.
4) The value computation of the built-in post-increment and post-decrement operators is sequenced before its side-effect.
5) The side effect of the built-in pre-increment and pre-decrement operators is sequenced before its value computation (implicit rule due to definition as compound assignment)
6) Every value computation and side effect of the first (left) argument of the built-in logical AND operator && and the built-in logical OR operator || is sequenced before every value computation and side effect of the second (right) argument.
7) Every value computation and side effect associated with the first expression in the conditional operator ?: is sequenced before every value computation and side effect associated with the second or third expression.
8) The side effect (modification of the left argument) of the built-in assignment operator and of all built-in compound assignment operators is sequenced after the value computation (but not the side effects) of both left and right arguments, and is sequenced before the value computation of the assignment expression (that is, before returning the reference to the modified object)
9) Every value computation and side effect of the first (left) argument of the built-in comma operator , is sequenced before every value computation and side effect of the second (right) argument.
10) In list-initialization, every value computation and side effect of a given initializer clause is sequenced before every value computation and side effect associated with any initializer clause that follows it in the brace-enclosed comma-separated list of initalizers.
11) A function call that is not sequenced before or sequenced after another expression evaluation outside of the function (possibly another function call) is indeterminately sequenced with respect to that evaluation (the program must behave as if the CPU instructions that constitute a function call were not interleaved with instructions constituting evaluations of other expressions, including other function calls, even if the function was inlined).
The rule 11 has one exception: function calls made by a standard library algorithm executing under std::execution::par_unseq execution policy are unsequenced and may be arbitrarily interleaved with each other.	(since C++17)
12) The call to the allocation function (operator new) is indeterminately sequenced with respect to (until C++17)sequenced before (since C++17) the evaluation of the constructor arguments in a new-expression
13) When returning from a function, copy-initialization of the temporary that is the result of evaluating the function call is sequenced-before the destruction of all temporaries at the end of the operand of the return statement, which, in turn, is sequenced-before the destruction of local variables of the block enclosing the return statement.
In a function-call expression, the expression that names the function is sequenced before every argument expression and every default argument.
15) In a function call, value computations and side effects of the initialization of every parameter are indeterminately sequenced with respect to value computations and side effects of any other parameter.
16) Every overloaded operator obeys the sequencing rules of the built-in operator it overloads when called using operator notation.
17) In a subscript expression E1[E2], every value computation and side-effect of E1 is sequenced before every value computation and side effect of E2
18) In a pointer-to-member expression E1.*E2 or E1->*E2, every value computation and side-effect of E1 is sequenced before every value computation and side effect of E2 (unless the dynamic type of E1 does not contain the member to which E2 refers)
19) In a shift operator expression E1<<E2 and E1>>E2, every value computation and side-effect of E1 is sequenced before every value computation and side effect of E2
20) In every simple assignment expression E1=E2 and every compound assignment expression E1@=E2, every value computation and side-effect of E2 is sequenced before every value computation and side effect of E1
21) Every expression in a comma-separated list of expressions in a parenthesized initializer is evaluated as if for a function call (indeterminately-sequenced)

expr(ex1,ex2) --> Burada argüman olan ifadelerin hesabından önce eğer expr de bir side effect veya hesaplama varsa
onlar daha önce yapılacak.Yani expr de bir işlem olabilir.expr bir func ptr array olup burada bir işlem olabilir.

new T(ex1,ex2,ex3) --> C++17 ile birlikte önce operator new çağrılacak, daha sonra ctora gönderilen argümanlar 
evaluate edilecek.

= Atama operatoru --> = nin sağ operandı olan ifade önce hesaplanıp yan etkileri tamamlanacak daha sonra sol
operand üzerinde yan etki oluşacak. C de UB olan bazu durumlar C++ta böyle olmuyor artık.

i = ++i + 2;       // undefined behavior until C++11
i = i++ + 2;       // undefined behavior until C++17
f(i = -2, i = -2); // undefined behavior until C++17
f(++i, ++i);       // undefined behavior until C++17, unspecified after C++17
i = ++i + i++;     // undefined behavior

cout << i << i++; // undefined behavior until C++17
a[i] = i++;       // undefined behavior until C++17
n = ++i + i;      // undefined behavior


MOLA

=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================

Biraz Alias Template lerden bahsedildi.
Bunun sebebi tür eş ismi bildirimini template haline getirmek.

template <typename T>
using TypePtr = T*;

Bu Temel C++ ta görüldü yazmayacağım.

=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================

Aynı sınıf şablonunun farklı template argümanlarıyla oluşturulmuş açılımları/specializationları farklı sınıflar.
Bunların arasında bir ilişkide yok.Myclass<int> bir template id, Myclass<double> ise ap ayrı.

template <typename T>
class Myclass{
	
};


Instantiation processi template için bir specialization oluşturuyor. Myclass<int> ve Myclass<double> ayrı specializationlar

Instantiation otomatik olabiliyor, birde explicit olabiliyor.

=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================
=========================================================================================================================================

TEMPLATE EXPLICIT INSTANTIATION
-------------------------------
Derleyici bazı durumlarda template koddan faydalanarak sınıf/funcın kodu yazıyor.Her durumda yazmıyor ama.
Nasıl incomplete type gibi kodun tamamının gerekmediği yerler varsa templatelerde de böyle.
Bir template ten fiilen specializationu oluşturmasını istiyoruz yani

template <typename T>
class Myclass;

int main()
{
	Myclass<int> *p; // Mesela incomplete type pointer değişken için yeterli.Burada bir instantiation yapmayacak
			 // incomplete type yeterliyse instantiation gerekemiyorsa derleyici template koddan bir
			 // specialization oluşturmayacak.
	
	Myclass<int> x;  // Burası ise sentaks hatası.Yukarıdaki sınıf complete type olmalı.
}

-----------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class Myclass
{
};

int main()
{ 
	Myclass<int> x; // Bu nesneyi tanımlayınca derleyici bir instantiation yapacak.Bunu yaptığı noktaya Point of Instantiation
			// deniyor. Yani kodu yazacağı nokta.Bu otomatik olarak yapılıyor. Derleyici bunu bize sormuyor.
			// Öyle bir sentaks varki derleyici instantiation yapmaya, point of instantiation oluşturup,
			// Derleyici bu şablondan bu specialization kodunu yaz diyebiliyoruz.
}

-----------------------------------------------------------------------------------------------------------------------------------------

EXPLICIT SPECIALIZATON İLE INSTANTIATIONI KARIŞTIRMA

template <typename T>
void func(T)
{
	
}

template void func<int>(int); // burası explicit instantiation.Yani kod burada olacak.

template void func<>(double); // Func ın double argümanı için bir specialization yaz demek.
			      // <> olduğundan deduction olacak.double çıkacak

template void func(float);    // Func ın float argümanı için bir specialization yaz demek.Açısal parantez kullanmasakta legal
			      // Derleyici bu specializationları bizim için instantiation edecek.

int main()
{
		
}

-----------------------------------------------------------------------------------------------------------------------------------------

ÖR:

template <typename T>
class Myclass{
public:
	void func(T)
	{
	
	}
	
	void foo(T, T)
	{
	
	}
};

// EXPLICIT INSTANTIATION TANIMI YAPALIM
template void Myclass<int>::func(int); // Myclass<int> için bunun tanımını yani instantiationını burada yaz. Tanımını yani.

template Myclass<int>; // Burada da myclass<int> in kendisi için tanım yaptırıyoruz.Yani sınıfın tamamı için

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

NEDEN BÖYLE BİR ŞEYE İHTİYAÇ VAR

Template kodlar header fileda olmalı.Çünkü derleyicinin instantiation yapabilmesi için bunları görmesi gerekiyor.

Şöyle bir yük var. Bu başlık dosyası birden fazla kod dosyasında include edildiğinden derleyici bunları ayrı ayrı derlediğinden,
o template kodların parse edilmesi ve instantiation süreci tekrar tekrar aynı template argümanları için sözkonusu olduğunda
her kaynak dosya derlendiğinde bu işlemler tekrar yapılıyor.Bu işlemler tekrar yapılıyor buda build time uzatıyor.
Tempalte lerden en fazla şikayet bu noktada.

Bunun için bazı teknikler kullanılabilir.Yani ayrı ayrı dosyalarda farklı farklı instantiationlar oluşturulmasını istemiyoruz.
Bu build time ı düşürmek için neler yapılabilir.Tekrar tekrar compile edildiğinde bu instantiation yükünü azaltabiliyoruz.

1. TEKNIK
---------
Bir bir dosya oluşturabiliriz. Deneme.h yarattık, buraya func şablonunun bildirimini koyuyoruz.

Deneme.h
--------
template <typename T>
void foo(T x);

Bunu compile edebiliriz tabi ama link edemeyiz.Bu ayrı bir durum.Link edebilmem için şablondan kodu instantiation edebilmesi gerekir.
Problem, ali.cpp, veli.cpp, hasan.cpp bu herkaynak dosya derlendiğinde bu süreç yeniden gerçekleşecek.

İkinci bir dosya oluşturuyoruz.Func şablonunun tanımını buraya koyuyoruz.
Deneme.hpp
----------
template<typename T>
void foo(T x)
{
	std::cout << "foo specialization for type " << typeid(T).name() << "\n";
}

Üçüncü bir Dosya
Deneme.cpp
----------
#include "Deneme.hpp"    

template void foo(int); // TEMPLATE EXPLICIT INSTANTIATION
			// DENEME.HPP INCLUDE EDILDIĞI IÇIN, DERLEYICI DENEME.CPP YI DERLEDIĞINDE FOO YU GÖRÜYOR.
			// ZATEN GÖRMESE INSTANTIATION YAPMA OLANAĞI YOKTU.DENEME.CPP NIN DERLENMESI DEMEK ARTIK
			// INSTANTIATIONUN YAPILMASI DEMEK

main.cpp
--------
#include "Deneme.h"

int main()
{
	foo(12); // DERLEYICI ŞUANDA BU NOKTADA SADECE BILDIRIM GÖRDÜĞÜNDEN INSTANTIATION YAPMADI AMA DENEME.CPP
		 // KAYNAK DOSYASINI DERLEDIĞINDE INSTANTIATION YAPTI VE LINK AŞAMASINDA KOD OLDUĞUNDA ARTIK BIR
		 // PROBLEM OLMAYACAK.ZATEN MAIN IÇIDE FOO NUN DECLERATIONI VAR, DENEME.H DEN INCLUDE ETTI.
		 // TEMPLATE DENEME.HPP DE, KODUN KENDISI YANI INSTANTIATION EDILMIŞ HALI ISE DENEME.CPP DE.
}


Yani bir dosyayı buna ayırsak.Deneme.cpp nin içne çok sık kullanılanları explicit instantiation sentaksı ile yazsak.
Deneme.hpp içinede yazabilirdik ama kod büyüdükçe bu zorlaşıyor.Link aşamasında hata alsak işin içinden çıkmak zor olurr.

Deneme.cpp
----------
#include "deneme.hpp"

template void foo(int);
template void foo(double);
template void foo(long); // BUNLARA EXPLICIT INSTANTIATION DEFINITIONDA DENIYOR.

-----------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------

2. TEKNIK
---------
EXPLICIT TEMPLATE INSTANTIATION DECLERATION.Burada sentaks biraz daha farklı. Modern C++ ta geldi.

main.cpp 
--------
template <typename T> void f()  //Bu func şablonunun tanımı
{

}

extern template void f<float>();   // extern template void f(float);  BU ŞEKİLDE YAZSAMDA OLUR

extern template void f<int>();  // ÖNCEKINDE EXTERN YOKTU.BU BILDIRIME EXPLICIT INSTANTIATION DECLERATION DENIYOR.
				// DERLEYICI BU BILDIRIMI GÖRDÜĞÜNDE, DERLEYICI BU FUNC ŞABLONUNDAN INT ARGÜMANI IÇIN 
				// OLUŞTURULACAK SPECIALIZATION IÇIN INSTANTIATION YAPMA.ÇÜNKÜ BAŞKA BIR MODÜLDE BUNUN INSTANTIATIONU YAPILMIŞ.
				// EĞER BURADA INSTANTIATION YAPARSAK BIRDEN FAZLA KEZ INSTANTIATION YAPILACAK. BÖYLECE DERLEYICIYE TEMPLATEIN TANIMINI
				// GÖRMESINE RAĞMEN INT SPECIALIZATIONU IÇIN INSTANTIATION YAPMAYACAK.
				// SONRASI KOLAY, BİR BAŞKA DOSYADA BU SPECİALİZATİONLAR İÇİN EXPLICIT INSTANTIATION DEFINITION VERECEĞİZ.
				// BÖYLECE BİR CPP DOSYASINDA BU ARGÜMANLAR İÇİN TANIM OLARAK AMA KODUN TAMAMI BAŞLIK DOSYASINA GELECEK.



ÖR:

t.hpp
-----
template <typename T> void f()
{
	
}

extern template void f<float>();	

extern template void f<int>(); // Explicit template instantiation decleration.Bu bildirim derleyiciye diyor ki f template inin int specializationu için sen 
				// instantiation yapmayacaksın, çünkü bunun instantiationu başka bir kaynak dosyada yapılıyor.
						

t.cpp
-----
#include "t.hpp"

template void f<int>; // definition
template void f<float>; // definition


main.cpp
--------
#include "t.hpp"

int main()
{
	f<int>(5);  // Derleyici yukarıdaki kodu gördü. Yukarıdaki decleration kodlarını görünce derleyici bir instantiation yapmama kararı aldı.
		   // derleyici func call kodunu üretti ama bu templatten kodu instantation etmedi.
}

BU ÖRNEĞİ IDE ÜZERİNDE TEST EDİYORUZ.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

STACK - QUEUE - PRIORITY QUEUE
------------------------------
Aslında STL de bunlar container adapter diye geçiyor. Compositionla bir containerı alıyorlar ve o containerın interfaceini adapte ederek abstract data type interface oluşturuyorlar.
Bunlar stack, queue ve priority queue

Stack Template
--------------
template <typename T, Typename C = std::deque<T>>
class stack
{
public:
	

private:
	C mcon;		// STL de bu protected interface te hatta
};

int main()
{
	Stack<int, std::vector<int>>; // Neden deque yerine vector kullanalım ???????????

	deque<int> mydeque{2,5,6,8,8,1,3}; // Buradaki deque i kullanarak verileri doğrudan stacke verebiliyoruz. Böyle bir ctor var.
									   // Bu ctor parametre olarak o private veri elemanı ile aynı türden container istiyor.

	stack<int> s{mydeque};

	std::cout << "s.size() = " << s.size() << "\n"; // 7

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	ELİMDE ZATEN BİR VECTOR VARSA

	vector<int> myvec{2,5,6,8,8,1,3};

	stack<int> s{myvec}; //Sentaks Hatası
	
	stack<int,vector<int>> s{myvec}; //Geçerli

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	stack<int> mystack;

	mystack.pop(); // Stack boş iken pop çağırmak Exception throw etmiyor. Neden void döndürüyor ? Exception safety için.
					// Diğer diller pop funcı çıkartılan öğenin değerini döndürüyor.

}

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

NOT: STL DEKİ SINIF ŞABLONUNDA C PARAMETRESİ YANİ DEQUE PARAMETRESİ PROTECTED INTERFACE TE YER ALIYOR.

class Mystack : public std::stack<int>
{
public:
	void foo()
	{
		c. // Burada kalıtımla stack ten yeni bir sınıf elde edince o container öğesi protected olduğundan taban sınıftaki c öğesine erişebiliriz.
	}
}


MOLA

====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================
====================================================================================================================================================================================

NOT : EXPLICIT TEMPLATE SPECIALIZATION ILE TEMPLATE EXPLICIT INSTANTIATION FARKLI ŞEYLER. AYNI SPECIALIZATION IÇIN HEM EXPLICIT TEMPLATE SPECIALIZATION HEMDE EXPLICIT INSTANTIATION
YAPAMAYIZ. ÇÜNKÜ HER IKISIDE ASLINDA TANIM.

STL STACK SINIFINA BAKALIM

int main()
{
	using namespace std;

	std::stack<int> s;

	s.push(1);
	s.push(2);
	s.push(3);

	s.top() = 12; // Geçerli.Çünkü referans dönüyor.

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	const std::stack<int> s;

	s.top() = 12; // SENTAKS HATASI

	auto a = s.top(); // GEÇERLİ
}

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

KALDIĞIMIZ YERDEN DEVAM EDIYORUZ.KENDI STACK SINIFIMIZI PRIMITIVE OLARAK YAZDIK.
Funcları inline olarak sınıf içinde de yazabiliriz.

#include <vector>
#include <iostream>
#include <cassert>
#include <stack>

template<typename T>
class Stack{
public:
	void pop();
	void push(const T&);
	bool empty()const
	{
		return c.empty();
	}
	void print()const
	{
		for(const auto &x :c )
		{
			std::cout << x << " ";
		}
	}
	const T& top();

private:
	std::vector<T> c;
};

template <typename T>
void Stack<T>::pop()
{
	// Sınıf şablonu içinde specializationun kendisini kullanmak için 2 ayrı sentaks kullanabiliriz.
	// 1 - Stack<T> x; // bu durumda bu kod instantiate edildiğinde, x in türüde Stack<T> açılımı olacak. T int ise Stack<int> olacak x türü mesela.  
	// 2 - Stack x; // Bu durumda da yukarıdaki ile aynı anlamda kullanılıyor. X in türü stack<int> açılımı için stack<int> olacak.Temel C++

	assert(!c.empty());

	c.pop_back();
}

// SENTAKS HATASI
//template <typename T>
//void foo(Stack x) //Burası ise sentaks hatası. Stack<T> x yazılabilir.
//{
//	
//}

template <typename T>
void Stack<T>::push(const T& val)
{
	assert(!c.empty());
	c.push_back(val);
}

template <typename T>
void Stack<T>::pop()
{
	 assert(!c.empty());
	 c.pop_back();
}

SORU : 
1 - BURADA INLINE KULLANMAMAK UNDEFINED BEHAVIOR MU ? TEMPLATE OLMASAYDI BU ODR İHLALİ YAPARDI AMA TEMPLATELERDE TOKEN BY TOKEN AYNI OLMASI DURUMUNDA ODR İHLAL EDİLMİYOR.
2 - INLINE KULLANMAK VE KULLANMAMAK ARASINDA FARK VAR MI ? INLINE AYNI ZAMANDA INLINE EXPANSION SAĞLIYOR AMA DERLEYİCİLER BUNU ÇOKTA ÖNEMSEMİYOR.DERLEYİCİ ZATEN YAPABİLİRSE YAPAR.

Bir funcca çağrı yapmadan derleyici o funcın kodunu instantiate etmiyor.

int main()
{
	Stack<int> s;

	s.push(12); //Derleyici push un kodunu yazdı.

	-----------------------------------------------------------------------------------------------------------------------------------------------------------------

	Stack<Myclass> s;

	Myclass m1, m2;

	s.push(m1);
	s.push(m2); // GEÇERLİ HATA YOK

	s.print(); // Burası sentaks haatası olur çünkü for içindeki x in türü myclass.Çünkü Myclass ın operator<< funcı yazılmadı.

}


Stack<int> kendimizde yazsak yada derleyici sınıf şablonundan bu specializationu instantiate etti. Bir fark yok.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BİLİNMESİ GEREKENLER ÖZET
-------------------------
Template Argument Deduction
CTAD
Deduction Guides
	(Implicit - Explicit)
	(C++20 için aggregate typelar için yenilikler var)
Template type parameter
Template non-type parameter
Template template parameter

Default Argument
Explicit Specialization
Partial Specialization
Explicit Instantiation Decleration
Explicit Instantiation Definition

Friend Declerations
Varidic Templates

Daha yazmadıklarıda var hocanın.

*/













#include "t.hpp"

//BU KISIMA t.hpp nin geldiğini düşünelim.

//template <typename T>
//void f()
//{
//	//... kodlar kodlar
//}
//
//extern template void f<float>();
//
//extern template void f<int>();



int main()
{
	f<int>(); // Derleyici yukarıdaki kodu gördü. Yukarıdaki decleration kodlarını görünce derleyici bir instantiation yapmama kararı aldı.
				// derleyici func call kodunu üretti ama bu templatten kodu instantation etmedi.
}
