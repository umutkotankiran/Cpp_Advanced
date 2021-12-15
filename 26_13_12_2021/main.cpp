/*

FORMATTAN DEVAM

[fill][align][sign][#][0][width][.prec][L][type]

iostreamin okumasıda zor çünkü formatlama özellikleri birbirinden ayrı yani tek bir string biçiminde değil,
ağırlıklı olarak maniplatorler veya setf, unsetf, width gibi functionları çağırıyoruz zaten kullandığımız maniplatörlerde
bu funcları çağırıyorlar.

format daha hızlı, daha okunabilir, lokazisayona uygun olması, format stringinin ayrı tutulabimesi...
Bunlar avantajlar.

Bir format_to func var. 
format ile format_to arasında bir fark var.Format bellek alanını kendisi temin ediyor, 
format_to ise bizim belirlediğimiz bellek alanında oluşturuyor formatlanmış stringi.

Formatın return değeri string. Normalde format functionuna gönderdiğimiz format stringi her ne kadar 
implementasyonlar arasında fark olsada, compile timeda elde edilebilecek bir yazı olması gerekiyor.
Compile time değilde runtimeda elde edilecekse vformat isimli functionu kullanıyoruz.


-----------------------------------------------------------------------------------------------------------------------------------

ÖR:

#include<format>
#include<iostream>

using namespace std;

int main()
{
	cout << format("{{ali}}") << '\n'; // "{ali}" yazdı
	
	----------------------------------------------------------------------------------------------
	
	int x{18};
	cout << format("{}", x) << '\n'; // 18 yazdı. Hiçbir özellik verilmezse herşey default formatlama özelliğinde oluyor
									 // birden fazla {} olsaydı hepsi sırayla bir argümanla eşleşecekti

	cout << format("{2} {0} {1}", x,"ali",x+5) << '\n'; // 23 18 ali
}										 

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

[fill][align][sign][#][0][width][.prec][L][type]

İNCE AYAR :'TEN SONRA GELİYOR
# in tek başına anlamı yok. Bu bazı formatlama özelliklerini modifiye ediyor.
Padding zero 0 ile doldurma karakterini karıştırmamak gerekiyor.
.prec precision hem stringler hemde reel sayılar için sözkonusu.Reel sayılarda . dan sonra kaç basamak yazılacağı,
stringlerde ise kaç karakter yazılacağının bilgisi.
L set edilmiş yada funca argüman olarak gönderilen localin özellikleriyle formatlama yapılsın demek.
type ta tamsayı reel sayı boolean character türlerinme ilişkin formatlama özelliğini temsil ediyor.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
Tamsayı Türleri İçin Formatlama Neler Olabilir ? 
aA -> Hexfloat. 0X...
bB -> Sayı sistemi tabanı büyük gösterir B 
c -> Character
d -> decimal
eE -> Exponancial. Üstel notasyon
fF -> fixed format
gG -> Standart Float 
o -> octal gösterim
p -> Pointer address hex representation
s -> copy string to output, or true false/bool
xX -> Hexadecimal sayı sistemi. Büyük gösterir X
	
Boolean yazdırmak için,
s -> True false yazar yani string olarak yazar.
bB -> 0 1 yazar
d -> 0 1
o -> 0 1 
xX -> 0 1 
c -> 0 1		Hepsi bool da kullanılabilir.
	
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖRNEKLER

int main()
{
	std::cout << format("{}\n", 10 > 5); // true
	std::cout << format("{:d}\n", 10 > 5); // 1
	std::cout << format("{}\n", 65); // 65
	std::cout << format("{:c}\n", 65); // A
	std::cout << format("{}\n", 'A'); // A

	string name {"murat hepeyiler"};
	cout << format("|{}|", name) << '\n'; // murat hepeyiler

	----------------------------------------------------------------------------------------------

	string name {"murat hepeyiler"};
	std::cout << format("{}",name); // murat hepeyiler
	std:cout << format("|{:24s}|",name) << "\n"; // |murat hepeyiler		|   Default alignment sola dayalı
	std:cout << format("|{:<24s}|",name) << "\n"; // |murat hepeyiler		|
	std:cout << format("|{:>24s}|",name) << "\n"; // |	 murat hepeyiler|
	std:cout << format("|{:^24s}|",name) << "\n"; // |   murat hepeyiler   |
	std:cout << format("|{:.^24s}|",name) << "\n"; // |.....murat hepeyiler.....|
	std:cout << format("|{:<24.5s}|",name) << "\n"; // |murat				|
	std:cout << format("|{:+>24.5s}|",name) << "\n"; // |+++++++++++++murat|
	std:cout << format("|{:!^24.5s}|",name) << "\n"; // |!!!!!!murat!!!!!!|

	----------------------------------------------------------------------------------------------

	std::ostream formatlamasından bir farklılık daha var

	int x{};
	std::cout << &x << "\n"; // burada void* parametreli func çağrılır
	
	std::cout << format("{}",&x) << "\n"; // Sentaks hatası. Eşlenen ifadenin void* türünden olması gerekiyor
	std::cout << format("{:p}", &x) << "\n"; //Sentaks hatası yine

	std::cout << format("{}",(void*)&x) << "\n"; // Geçerli artık
	std::cout << format("{:p}",(void*)&x) << "\n"; // Yine geçerli artık
	std::cout << format("{:p}", static_cast<void *>(&x)) << "\n";  // Dikkat !!! {:p} var
	
	std::cout << format("{}", nullptr) << "\n"; // 0x0
	std::cout << format("{:p}", nullptr) << "\n"; // 0x0

	----------------------------------------------------------------------------------------------
	
	#'E BAKALIM
	Tamsayılarda sayı sistemi tabanını yazmaya zorluyor.

	std::cout << format("{:x}",47802) << '\n'; // baba 
	std::cout << format("{:X}",47802) << '\n'; // BABA
	std::cout << format("{:#x}",47802) << '\n'; // 0xbaba
	std::cout << format("{:#X}",47802) << '\n'; // 0XBABA
	std::cout << format("{:b}",47802) << '\n'; // 10111010100..0
	std::cout << format("{:#b}",47802) << '\n'; // 0b10111010100..0
	std::cout << format("{:o}",47802) << '\n'; // 135272
	std::cout << format("{:#o}",47802) << '\n'; // 0135272

	std::cout << format("{:f}",5.) << '\n'; //
	std::cout << format("{:#f}",5.) << '\n'; // 

	----------------------------------------------------------------------------------------------

	Nested PlaceHolder Örneği

	double dval = 3. / 7;
	int width = 12;
	int precision = 5;

	std::cout << format("|{2:<{0}.{1}f}|",width,prec,dval);    //  |			0.42857|


}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LOCALE
Format functionunun 1. parametresine doğrudan locale nesnesini gönderebiliriz.
Bu ayrı bir overload tabiki.
	
#include<format>
#include<iostream>
#include<string>
#include <locale>

int main()
{
	std::cout << format(locale{"turkish"}, "{:L}",3.12) << '\n';  // 3,12  ondalık ayıraç virgül Türkçede
	std::cout << format(locale{"turkish"}, "{:L}",456'877'123 )<< '\n';  //   456.877.123 
	std::cout << format(locale{"turkish"}, "{}",456'877'123 )<< '\n';  //   456877123 
	std::cout << format("{}",456'877'123 )<< '\n';  //    456877123
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LOCAL GLOBAL OLARAKTA DEĞİŞTİRİLEBİLİYOR.

#include<format>
#include<iostream>
#include<string>
#include <locale>

int main()
{
	locale::global(locale{"turkish"}); // Burada değiştirildi.

	std::cout << 123.96585 << '\n'; // 123.96585

	std::cout << format("{:L}\n", 4.56) << '\n';  // 4,45
}

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FORMAT_TO
---------
Format ile arasıdanki fark, format bir string oluşturuyor ama format_to ilk parametresine geçtiğimiz iteratörü
kullanarak o iteratörden başlayarak karakterleri yazıyor. Dolayısı ile bir string oluşturmasını istemezsek,
o stringi kullanmayacaksak, doğrudan insert işlemi yapacaksak veya bir doğrudan containera koyacaksak veya
bir stringi dolduracaksak format yerine format_to kullanılabilir.

#include<format>
#include<iostream>
#include<string>
#include<locale>
#include<iterator>
#include<fstream>
#include<cmath>


int main()
{
	int x = 47802;

	format_to(ostream_iterator<char>(cout),"|{:>16X}|", x); // |		BABA|

	---------------------------------------------------------------------------------------------

	ofstream ofs{"output.txt"};
	if(!ofs)
	{
		cerr << "dosya olusturulamadi\n";
		return 1;
	}
	
	for(int i = 0; i < 1000; ++i)
	{
		//format_to(ostream_iterator<char>(), "{:>6} {}",i,sqrt(i));
		//format_to(ostream_iterator<char>(), "{:<6} {}",i,sqrt(i));
		format_to(ostream_iterator<char>(), "{:<6} {:>12.4f}",i,sqrt(i)); // sağa dayalı, 12 karakterlik alanda noktadan sonra 4 character fixed notasyon
	}
}

--------------------------------------------------------------------------------------------------------------------

std::back_inserter

back_inserter kapasiteyi artırıyordu.

int main()
{
	string name{"Harun Bozaci"};

	int id{78945};
	double dval{54.213455};

	string str;

	format_to(back_inserter(str),"|{}{}{:.2f}|",id,name,dval); // stringe insert etti.back_inserter bir back_insert_iterator veriyor.
																// bu iterator konumuna yazınca, ctoruna verdiğimiz containerın aslında 
																// push_back functionu çalışıyor.

	std::cout << str << '\n';

	------------------------------------------------------------------------------------

	list<char> clist;

	format_to(front_inserter(clist),"|{}{}{:.2f}|",id,name,dval); // liste insert etti.
	std::cout << clist << '\n';

	for(auto c : clist)
		cout << c;

}

--------------------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	string str;

	for(char c = 'A'; c <= 'Z'; ++c)
	{
		format_to(back_inserter(str),"{{{0}, {0}}}\n",c);
	}

	-------------------------------------------------------------------------------------
	
	std::cout << str << "\n";
	for(char c = 'A'; c <= 'Z'; ++c)
	{
		format_to(back_inserter(str),"{{{0}, {0:d}}}\n",c); // karaktere karşılık gelen kodu yazdırdık
	}

	std::cout << str << "\n";

}

--------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------

FORMAT_TO_N
-----------
Bir iteratörün konumundan iteratörün gösteridiği yere değil!!!, doğrudan verdiğimiz adrese yazıyor.
Bir avantajı buffer overflow yani diziyi taşırmayacak o yüzden güvenli.
Return değeri format_to_n_result türünden struct. Bu struct'ın 2 elemanı var biri adres diğeri tamsayı.
Adres olanı kullanınca(out elemanı) bu yazının sonuna nullchar koymuyor.Bİzim koymamız gerekiyor.
Bunun için kaç byte okuduğunu bilmemiz gerekiyor. Buda kaç karakter yazdığına bağlı.Bu kaç karakter yazdığınıda
format to n in return değeri olan yapı nesnesinin elemanlarından elde edebiliyoruz.

int main()
{
	string name{"necati ergin"};
	char buffer[64];

	// 1. parametre yazılacak adres, 2. parametre yazılacak karakter sayısı, 3. param formatlama stringi.
	auto result = format_to_n(buffer,std::size(buffer) - 1, "{} isminin uzunluğu {}.", name,name.lenght());

	std::cout << buffer << '\n'; //UB çünkü sonda nullchar yok.

	-----------------------------------------------------------------------------------------------------------

	// result.out  --> ADRES.
	// result.size // tamsayı.kaç byte okunduğpu bilgisi

	-----------------------------------------------------------------------------------------------------------

	std::cout << result.size << "\n"; // 33. kaç karakter yazdığı bilgisi.
	std::cout << result.out - buffer << "\n"; // 33

	-----------------------------------------------------------------------------------------------------------

	*result.out = '\0'; // yada buffer[result.size] = '\0';
	 
	std::cout << "|" << buffer << "|";
}

MOLA

=============================================================================================================================
=============================================================================================================================
=============================================================================================================================

int main()
{
	//std::array<char,10> ar; // Şuanda garbage valueda
	
	std::array<char,10> ar{}; // Şuanda hepsi 0 değerde.

	for(auto c : ar)
		cout << (int)c << ' ';

}

-------------------------------------------------------------------------------------------

int main()
{
	std::array<char,10> ar{};

	format_to_n(ar.data(), ar.size() - 1,"{} {}", "Necati","Ergin");

	cout << "ar.data() : " << ar.data() << '\n'; // Necati Er
}

-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------

STD::FORMATTED_SIZE
-------------------
Formatlama sonucu oluşacak stringin size'ını döndürüyor ama stringi oluşturmuyor.

int main()
{
	string name{"ahmet aksoy"};
	int age{25};

	auto n = formatted_size("{} {}", name,age); // Formatlama sonucu oluşacak yazı uzunluğu n değerinde 
	std::cout << "n = " << n << '\n'; // 14
}

-------------------------------------------------------------------------------------------

Bu değeri alıp o kadar karakterlik bir yer ayırma işleminde kullanabiliriz.
Containerda reserve ile yer ayırabiliriz vectorde.

int main()
{
	int x {65234};
	double dval = 123.654;

	const char *p{"{:#X} {:.2f}"}; // Tekrar kullanacağımız için bunu bir pointerda tuttuk.
	const auto n = formatted_size(p,x,dval); // formatlanmış yazısının size'ını aldık.

	vector<char> buf(n); // n adet char ile oluşturdu vectorü
	format_to(buf.begin(), p,x,dval);

	for(auto c : buf)
		cout << c << " ";

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Formatta şuanda birsürü eksik var, daha print functionı yok.Zverovich in yazdıklarının yarısını ancak almışlar.
Microsoft derleyicisine göre Gcc geriden geliyor.

Format string compile değilde runtimeda elde ediliyorsa o zaman vformat kullanılıyor.
Argümanlarıda make_format_args isimli bir fabrika fonksiyonuna oluşturtuyoruz.

using namespace std;

const char * get_fmt_str();

int main()
{
	auto pfm = get_fmt_str();

	try{
		std::cout << vformat(pfm, make_format_args(87)) << "\n"; //          87
	}
	catch(const std::exception&ex)
	{
		std::cout << ex.what() << '\n';
	}
}

const char* get_fmt_str()
{
	return "{:16X}";
}

================================================================================================================================================================================================
================================================================================================================================================================================================
================================================================================================================================================================================================


CUSTOM FORMATLAMA
-----------------
Biraz sıkıntılı şuanda. Kimse bu kısmı sevmiyor çünkü fazla karmaşık ve gereksiz kod kalabalığı var.
placeholderı parse edecek parse func yazılacak.
Formatlamayı yapacak format func yazılacak.


struct Point{	// aggregate
	int mx,my;
};

STANDARTLAR DİYORKİ KENDİ TÜRÜN EXPLICIT SPECIALIZATION YAPACAKSIN
2 tane func tanımlayacağız ayrıca. 1. parse , 2. format isimli funclar
parse {} placeholderı parse edecek, format ise gerçek formatlamayı yapacak.
formatı yaparkende format_to çağrılacak.
AŞAĞIDA YAZDIĞIMIZ KODDA # VARSA KÜME PARANTEZİ İÇİNDE YAZACAK YOKSA NORMAL PARANTEZ !!!!!!!!!!!!!


template<>
class std::formatter<Point>
{
public:
	// typename std::format_parse_context::iterator bunun yerine auto yazacağız.Hatta func parametesine C++20 de auto kullanmakta geldi.
	auto parse(std::format_parse_context& pc)
	{
		auto iter = pc.begin();
		for(; iter != pc.end() && *iter != '}'; ++iter) // Şuanda placeholder içini dolaşıyoruz.En son } yada end pozisyonuun göreceğiz.
		{
			if(*iter == '#')  // biz burada # kabul ediyoruz diğerlerini karmaşık olmasın diye yazmadık.
			{
				cb_ = true; 
			}
			else
			{
				throw std::format_error{"invalid formatting character!"};
			}
		}
		return iter;
	}

	//BURADA FORMAT FUNCI TANIMLIYORUZ
	typename std::format_context::iterator format(const Point &p, std::format_context fc)
	{
		return std::format_to(fc.out(), "{}{}, {}{}", (cb_ ? '{' : '('), p.mx, p.my, (cb_ ? '}' : ')'));
	}

private:
	bool cb_{};
};

int main()
{
	Point p1{2,6} p2{3,5};

	std::cout << std::format("{:#} {}", p1,p2) << '\n';
}

ÇIKTI
-----
{2,6}  (2,5)


MOLA

==================================================================================================================================================================================================
==================================================================================================================================================================================================
==================================================================================================================================================================================================

Format daha tam implemente edilmedi.Burada havada kalan bazı konular var gibi.Daha derleyiciler bile
implemente etmemiş.

BİR ÖRNEK DAHA

3 formatlama özelliği olacak
1. Sadece isim kullanılacak
2. Sadece id 
3. İkisinin birlikte olması özellikleri olacak

class Person{
public:
	Person(std::string name, int id) : m_name(std::move(name)), m_id(id) { }
	
	//sınıfın get funcları olacak
	std::string get_name()const{ return m_name;}
	int get_id()const{ return m_id;}

private:
	std::string m_name;
	int m_id;
};

template<>
class std::formatter<Person>{
public:
	constexpr auto parse(auto &context)
	{
		auto iter{context.begin()};
		const auto iter_end{ context.end() };

		if(iter == iter_end || *iter == '}') // Eğer birşey girilmemişse default olma durumu
		{
			m_ftype = FormatType::All;
			return iter;
		}

		switch(*iter)
		{
			//a all için, isim için n, id için i olacak.
			case 'n' : f_type = FormatType::Name; break; 
			case 'i' : f_type = FormatType::id; break; 
			case 'a' : f_type = FormatType::All; break; 
			default : throw std::format_error{"Person format error!"};
		}
		
		++iter;

		if(iter != iter_end && *iter != '}') // Burası hata durumu
		{
			throw std::format_error{"Person format error!"};
		}
		return iter;
	}

	constexpr auto format(const Person &per, auto &context)
	{
		using enum FormatType; // FormatType::name yerine doğrudan name kullanabilşirim artık.İsimleri görünür yaptık.
		switch(m_ftype)
		{
			case Name : return std::format_to(context.out(), "{}",per.get_name());
			case Name : return std::format_to(context.out(), "{}",per.get_id());
			case Name : return std::format_to(context.out(), "[{}, {}]",per.get_id(),per.get_name());
		}
	}

private:
	//Formatlamadaki 3 seçeneği tutmak için bir enum türü oluşturuyoruz
	enum class FormatType{Name, Id, All};

	FormatType m_ftype;
};

int main()
{
	Person p1{"necati",876};
	Person p2{"harun",567};
	Person p2{"murat",789};

	std::cout << std::format("{:n}\n{:i}\n{:a}",p1,p2,p3);
}
necati
567
[murat,789]

-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------

C++20

1 - Aggregateler ile ilgili bir hata vardı geçmişte onu hallettiler.

struct Nec{
	int i{7};
	Nec() = delete;  //Eskiden ctor delete/defalt edilirse(user declared) bu durumda aggregate olma özelliği bozulmuyordu
};					  // User provided Ctorda bozmuyordu hatta

Aggreagte olduğunda aggregate init yapılabiliyor.Olmazsa eğer sentaks hatası

int main()
{
	Nec x; //SENTAKS HATASI

	Nec y{}; // C++17 de GEÇERLİ !!!! BU REZALET :D:D:D LEGAL OLMAMALI

}

Agregate olma koşullarını C++20 de değiştirdiler. Normalde yapılan eklemeler geçmişte yazılan kodların çalışmasını sağlayacak
şekilde ekleniyor.Artık C++20 de user declared olmamak zorunda yoksa aggregate olmaz.

struct A{
	A() = delete;
};

struct B{
	int x{1};
	B(int) = delete;
};

struct C
{
	int x;
	C();
};

C::C() = default;

int main()
{
	A a; // HATA
	B b{12}; //HATA. C++17 de geçerliydi
	
	//HİÇBİRİ GEÇERLİ DEĞİL
}

Özetle aggregate olma koşulları daha sık hale getirildi.

-----------------------------------------------------------------------------------------------------------------------------
 
2 - Aggregate initializationda sadece {} paramtezleri kullanılıyordu şimdi () lerde kullanılıyor. Bu parantezi seven parantez kulansın
diye değil. Bu ciddi bir problemdi.

struct Nec{
	int x,y;

};

int main()
{
	Nec nec1{10,20}; // LEGAl
	Nec nec2(10,20);  // C++20 İLE BUDA LEGAL
}

- Öyle yerer varki {} kullanınca sentaks hatası oluyor mesela perfect forwarding. Bir yapı türünü perfect forwarding
  edince sentaks hatası oluşuyordu. 

- Birde Macrolarda hatalar oluşuyor.

#include<cassert>

int main()
{
	using namespace std;

	assert(Nec{1,5}); // Macroda virgül kullanımı macro açılımını bozacak.Parantez olsaydı sentaks bozulmayacaktı
	assert(Nec(1,5)); // Bu Macro açılımını bozmuyor.
}


NOT : Aşağıdakilerde geçerli
int a[]{1,2,3};
int a[](1,2,3);




*/