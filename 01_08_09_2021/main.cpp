/*

İLERİ C++ İLK DERS
GENEL TEKRARLAR YAPILACAK BU SEBEPLE ÇOK NOT ALMADIM.
BU DERSTE ANLATILAN ŞEYLERİN BUYÜK ÇOĞUNLUĞU TEMEL C++ TA VAR.

Başlangıç konusu Taşıma Semantiği / Move Semantics

C++11 de geldi move semantics. C++11 ve C++20 önemli değişilikleri getiren standartlar.AYnı şekilde C++17 de önemli
Eski C++ a ancient C++ deniyor. C++ overcomplicated bir dil. 

Hemen her araç değer kategorisiyle / value category ilgili hale geliyor bu dilde.

Value kategori ifadeyi niteleyen bir terim
int x; / bu değişkenin value kategirisi ne demek saçma bir soru.
Bu bir değişkenle ilgili değil. Value category ifadeleri niteleyen terim.

İfadenin 2 önemli niteliği var. 
İfadenin türü
Değer kategorisi

ÖR:
x + 10 bunun bir type ı var birde değer kategorisi var.
Değişken türü referans türü olabilir pointer türü olabilir ama ifadelerin türü referans türü OLAMAZ.
İfadelerin referans olmayuan bir türü vardır birde değer kategorisi vardır. Bunlar farklı şeyler.

Bir ifadenin türü int, myclass, myclass * ,int * olabilr.

İfadenin değer kategorisi, o ifadenin dilin kurallarına göre nerelerde kullanılabileceğini ve kullanımmlarının ne ifade edeceğini belirliyor.

Bir ifadenin değer kategorisi 3 farklı kategoriden birine ait olmak zorunda. Bunlara primary value categories deniyor.
Lvalue Expression PrValue Expression Xvalue Expression. BUnlardan ikisine birden ait olamaz.

Buradaki şekili yazmayacağım zaten yazmıştım C++ ta.

glvalue : bu ifade bir nesneye yada bir kimliğe karşılık geliyormu
lvalue  : glvalue ama x value değil. kaynağı güvenli olarak çalınamayan ifadeler.
prvalue : bir ifadenin değerini hesaplamak veya bir nesneyi initialize etmek için kullanılıyor.kaynağı çalınabilir güvenli şekilde.
xvalue  : Taşıma semantiği ile eklendi.Öyle bir glvalue ki kaynakları tekrar kullanılabilir anlamında. expiring value

Kısaca decltype a girildi.

int&& func();

decltype(func()); Burası XVALUE expression

C++ notlarında var.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Bir ifadenin değer kategorisini yazdıracağız.

template<typename T>
struct Valcat
{
	constexpr static const char *p = "PR Value";
};

template<typename T>
struct Valcat<T&>
{
	constexpr static const char *p = "L Value";
};

template<typename T>
struct Valcat<T&&>
{
	constexpr static const char *p = "X Value";
};


int main()
{
	int x = 10;

	cout << Valcat<decltype((x))>::p << "\";	// İfadenin türüne göre yazdırıyoruz. Lvalue expr
	cout << Valcat<decltype(x + 5)>::p << "\";	// İfadenin türüne göre yazdırıyoruz. Rvalue expr

}


----------------------------------------------------------------------------------------------------------------------------------------------------------------

Birde functional macro yazdı ve kullandı normalde pek kullanılamaz ama.

template<typename T>
struct Valcat
{
	constexpr static const char *p = "PR Value";
};

template<typename T>
struct Valcat<T&>
{
	constexpr static const char *p = "L Value";
};

template<typename T>
struct Valcat<T&&>
{
	constexpr static const char *p = "X Value";
};

#define pvcat(#)		std::cout << "value category of : '" << #e << "' is " << Valcat<decltype((e))>::p << '\n'

int f1();
int& f2();
int&& f3();

int main()
{
	int x = 10;

	pvcat(x); //lvalue
	pvcat(x + 5); //prvalue

	pvcat(f1()); // Prvalue
	pvcat(f2()); // Lvalue
	pvcat(f3()); // Xvalue

}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

İSİM FORMUNDAKİ TÜM İFADELER LVALUE EXPRESSİONDUR.TÜRÜNDEN BAĞIMSIZ.

Lvalue Expression
-----------------
string literalleri
int x = 4;
int &r = x;
r -> lvalue expression Lvalue reference

int &&r = 23;
r-> lvalue expression ama Rvalue reference

[], * , = ile oluşturulanlar lvalue epxr. global func isimleri lvalue expr.
? : ün 2. ve 3. operandları aynı türden nesneler ise lvalue expr.
prefix ++ -- ile oluşturulan ifadeler lvalue expr
, ün sağ operandı lvalue expr ise bunun ürettiği değer lvalue expr. pvcat((x,y));


PR value epxression ise diğer operatörler ile oluşturulan operatörler.
----------------------------------------------------------------------
Karakter sabitleri 
tüm sabit ifadeleri
pvcat(-x);
pvcat(+x);
pvcat(x);
....

Xvalue expression
-----------------
Bunlar birkaç tane.Hatta visual studio bir tanesinde de hatalı gösteriyor.

- Return değeri sağ taraf referansı olan functionlara yapılan çağrı ifadeleridir.
- Move un return eğeride X value expr.
- PRvalue expression olan bir nesnenin nonstatic veri elemanına erişmek xvalue expressiondur.


struct Nec{
	int x;
};

Nec{}.x; // Xvalue Expression. Sınıf nesnesi geçici nesne yani prvalue expression.Elemanına erişme ifadesi xvalue expression

pvcat(Nec{}.x); // xvalue epxr

pvcat(void()); // PR value expr.
pvcat(nullptr); //prvalue expr

using Ar5 = int[5];

pvcat(Ar5{}(2)); // PRVALUE EXPR. VISUAL STUDIO DERLEYICISI BURADA SAÇMALIYOR.LVALUE EPXR ÇIKIYOR. BUNUNLA İLGİLİ BAYA UYARMIŞLAR.
GCC VE CLANG TE PRVALUE EXP GÖSTERİYOR.

cppreference tan value category den bakıladabilir. Baya detaylı yazmış.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

LVALUE
------
The following expressions are lvalue expressions:

the name of a variable, a function, a template parameter object (since C++20), or a data member, regardless of type, such as std::cin or std::endl. Even if the variable's type is rvalue reference, the expression consisting of its name is an lvalue expression;
a function call or an overloaded operator expression, whose return type is lvalue reference, such as std::getline(std::cin, str), std::cout << 1, str1 = str2, or ++it;
a = b, a += b, a %= b, and all other built-in assignment and compound assignment expressions;
++a and --a, the built-in pre-increment and pre-decrement expressions;
*p, the built-in indirection expression;
a[n] and p[n], the built-in subscript expressions, where one operand in a[n] is an array lvalue (since C++11);
a.m, the member of object expression, except where m is a member enumerator or a non-static member function, or where a is an rvalue and m is a non-static data member of object type;
p->m, the built-in member of pointer expression, except where m is a member enumerator or a non-static member function;
a.*mp, the pointer to member of object expression, where a is an lvalue and mp is a pointer to data member;
p->*mp, the built-in pointer to member of pointer expression, where mp is a pointer to data member;
a, b, the built-in comma expression, where b is an lvalue;
a ? b : c, the ternary conditional expression for certain b and c (e.g., when both are lvalues of the same type, but see definition for detail);
a string literal, such as "Hello, world!";
a cast expression to lvalue reference type, such as static_cast<int&>(x);
a function call or an overloaded operator expression, whose return type is rvalue reference to function;
a cast expression to rvalue reference to function type, such as static_cast<void (&&)(int)>(x).
(since C++11)
Properties:

Same as glvalue (below).
Address of an lvalue may be taken by built-in address-of operator: &++i[1] and &std::endl are valid expressions.
A modifiable lvalue may be used as the left-hand operand of the built-in assignment and compound assignment operators.
An lvalue may be used to initialize an lvalue reference; this associates a new name with the object identified by the expression.


PRVALUE
-------
The following expressions are prvalue expressions:

a literal (except for string literal), such as 42, true or nullptr;
a function call or an overloaded operator expression, whose return type is non-reference, such as str.substr(1, 2), str1 + str2, or it++;
a++ and a--, the built-in post-increment and post-decrement expressions;
a + b, a % b, a & b, a << b, and all other built-in arithmetic expressions;
a && b, a || b, !a, the built-in logical expressions;
a < b, a == b, a >= b, and all other built-in comparison expressions;
&a, the built-in address-of expression;
a.m, the member of object expression, where m is a member enumerator or a non-static member function[2], or where a is an rvalue and m is a non-static data member of non-reference type (until C++11);
p->m, the built-in member of pointer expression, where m is a member enumerator or a non-static member function[2];
a.*mp, the pointer to member of object expression, where mp is a pointer to member function[2], or where a is an rvalue and mp is a pointer to data member (until C++11);
p->*mp, the built-in pointer to member of pointer expression, where mp is a pointer to member function[2];
a, b, the built-in comma expression, where b is an rvalue;
a ? b : c, the ternary conditional expression for certain b and c (see definition for detail);
a cast expression to non-reference type, such as static_cast<double>(x), std::string{}, or (int)42;
the this pointer;
an enumerator;
non-type template parameter unless its type is a class or (since C++20) an lvalue reference type;
a lambda expression, such as [](int x){ return x * x; };
(since C++11)
a requires-expression, such as requires (T i) { typename T::type; };
a specialization of a concept, such as std::equality_comparable<int>.
(since C++20)
Properties:

Same as rvalue (below).
A prvalue cannot be polymorphic: the dynamic type of the object it denotes is always the type of the expression.
A non-class non-array prvalue cannot be cv-qualified, unless it is materialized in order to be bound to a reference to a cv-qualified type (since C++17). (Note: a function call or cast expression may result in a prvalue of non-class cv-qualified type, but the cv-qualifier is generally immediately stripped out.)
A prvalue cannot have incomplete type (except for type void, see below, or when used in decltype specifier)
A prvalue cannot have abstract class type or an array thereof.



XVALUE
------
The following expressions are xvalue expressions:

a function call or an overloaded operator expression, whose return type is rvalue reference to object, such as std::move(x);
a[n], the built-in subscript expression, where one operand is an array rvalue;
a.m, the member of object expression, where a is an rvalue and m is a non-static data member of non-reference type;
a.*mp, the pointer to member of object expression, where a is an rvalue and mp is a pointer to data member;
a ? b : c, the ternary conditional expression for certain b and c (see definition for detail);
a cast expression to rvalue reference to object type, such as static_cast<char&&>(x);
any expression that designates a temporary object, after temporary materialization.
(since C++17)
Properties:

Same as rvalue (below).
Same as glvalue (below).
In particular, like all rvalues, xvalues bind to rvalue references, and like all glvalues, xvalues may be polymorphic, and non-class xvalues may be cv-qualified.

GLVALUE VE RVALUE DA VAR YAZMADIM ONLARI
CPPREFERENCE TA BAYA DETAYLI AÇIKLAMALAR VAR. VOID TÜRÜ FALAN GİBİ... &STD::MOVE(X) GİBİ...

----------------------------------------------------------------------------------------------------------------------------------------------------------------

TÜRÜ ANLAMANIN BAŞKA YOLU

#include<type_traits>

int&& foo(); // bunada bakabiliriz.

int main()
{
	int x{};

	//aşağıda x yazan yere x + 4 veya başka şeyler yazıp test edebiliriz. Macro yazacaktı hoca uzamasın diye yapmadı.

	if constexpr(std::is_lvalue_reference<decltype_v((x))>)
	{
		std::cout << "Lvalue expr\n";
	}
	else if constexpr(std::is_rvalue_reference<decltype_v((x))>)
	{
		std::cout << "Rvalue expr\n";		
	}
	else
		std::cout << "prvalue expr\n";
}


not : Decltype, noexcept, sizeof, typeid bunlar unevaluated context

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Lvalue expr ların bellekte yeri var yani & in operandı olabilirler.

&(std::move(x)); // BURASI HATA

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Value Category

R Value Reference. Eski C++ ta 2 tane problem vardı. C++11 de iyi kötü çözüldü ama enteresan olan nokta aslında birbiriyle doğrudan ilişkisi olmayan iki sorunun
problemi için dile Sağ Taraf Referansları eklendi. Move Semantics ve Perfect Forwarding.

Kısaca move semantics anlatıldı. Kaynak çalma olayı.
Rvalue Reference ile bağlantısı.

C++11 ile Referans kategorileri geldi
Lvalue Reference : Sol taraf değeri olan ifadelere bağlanıyor.
Rvalue Reference : Sağ taraf değeri olan ifadelere bağlanıyor.
Xvalue Reference : ...

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Sınıfın special member funclarına girildi.
Derleyicinin kodlarını bizim için yazabiliyordu. Yazmıyorum eski konular.

Not Declared
User Declared
İmplicitly Declared

Derleyicinin implicitly delete etmesi durumları... private func çağırmak gibi, cosnt bir nesneye ilk değer vermemek gibi, referansa ilk değer vermemek, delete
edilmiş funa a çağrı gibi durumlarda compiler çağırması gereken/default etmesi gereken funcları delete ediyor.

Bizim special member tablosundan ilerlendi.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Sağ taraf referansları const olabilir ama kullanılma durumu hemen herzaman referansın bağlandığı nesneyi değiştirmeye yönelik olduğundan const sağ taraf referansı kullanmıyoruz.
const Myclass&& bunu move etmek UB Değil.

2:20 de örneği var ben yazmıyorum.
Yİne temel C++ konuları 3 tane overload yazdı hoca.

&
const&
&&
parametreli üç fonksiyon nasıl çağrılır ... gibi.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

int& foo(); // sınıf türünden olabilir.

int main()
{
	foo() = 12 ; //yapılabilir.
}

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Bir fonksiyonun parametresinin sağ taraf referansı olması onun taşıma yaptığı anlamına geliyor.
Move da taşıma yapan func değil ayrıca. Move doesnt move. Move bir nesneyi tür dönüşüm işlemi yaparak taşımaya aday hale getiriyor.

Myclass mx;

Yani AŞağıdakiler aynı anlamdalar

func(static_cast<Myclass&&>(mx));  
func(std::move(mx)); 




*/

#include <type_traits>
#include <string>
#include <iostream>

class Myclass {
public:
	Myclass() {}
	Myclass(const Myclass& other) : mx{ other.mx }, str{other.str} { }
	void set_value(int ival, std::string s)
	{
		mx = ival;
		str = s;
	}

	std::ostream& operator<<(std::ostream& os)
	{
		return os << mx << " " << str;
	}

private:
	int mx;
	std::string str;
};

int main()
{


}
