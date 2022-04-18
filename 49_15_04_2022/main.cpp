/*

En son JThread i görmüştük. thread tam bir raii sınıfı olmaya aday ama 
threadin destructoru join işlemini yapmıyor.

Jthreadi eklediler sonunda.Jthread sadece thread sarmalaması değil aynı zamanda thread sonlandırma
özelliğide eklediler. Sonra görülecek.
JThread RAII görevi görüyor ama threadi sonlandırma özelliğide eklediler.

-----------------------------------------------------------------------------------------------------

THREADLER ILE EXCEPTION HANDLING ARASINDAKI ILIŞKIYE BAKACAĞIZ

Bir threadi ya join edeceğiz yada detach edeceğiz.Yoksa terminate çağrılır.
Detach arka planda bağımsız olarak çalıştırmak için var

void foo()
{
	std::cout << "foo called\n";
}

void func()
{
	std::cout << "func called\n";
	throw std::runtime_error{"runtime error exception from func\n"};
}


int main()
{
	std::thread t{foo};

	func();  // Joini çağırmadan buradadaki kodda bir exception gönderilirse programın akışı
		// alttaki joine gelmeyecek. Joine gelmediği için join çağrılmayacak. Bu durumda thread
		// nesnesinin dtoru çağrıldığında joinable durumda olduğu için terminate func çağrılacak.

	t.join();
}


YUKARIDAKİ DURUMLA BAŞA ÇIKMAK İÇİN(JTHREAD VEYA KENDİ SINIFIMIZI YAZMADIYSAK) AŞAĞIDAKİ YAPILIR.

int main()
{
	std::thread t{foo};

	try{
		func();  // Burada hata olmazsa alt satıra geçer ve join ederiz.
		t.join();
	}
	catch(const std::exception&ex){
		std::cout << "exception caught : " << ex.what() << '\n';
		t.join(); // exception yakalanırsa yine burada join edeceğiz.çünkü yukarıda join edilmeden bu bloğa girer akış.
	}
	
	//..
}

Burada görülüyorki jthreade çok ihtiyaç var.

-----------------------------------------------------------------------------------------------------

Threadin çalıştırdığı function içinde(callable içinde) exception gönderilirse.

void func()
{
	std::cout << "func cagrildi\n";
	if(1)
	{
		throw std::runtime_error{"error from func\n"};
	}
}

void my_terminate()
{
	std::cout << "My terminate\n";
	abort();
}

int main()
{
	set_terminate(my_terminate);
	try{
		std::thread t{func}; // Buradan exception gönderiliyor ama yakalanmıyor.
				     // Doğrudan terminate çağrılıyor. Workload olarak verilen functa hata olunca
				     // hata yakalanmıyor. Yani exception gönderildiğinde doğrudan terminate çağrıldı.
		t.join();
	}
	catch(const std::exception &ex)
	{
		std::cout << ex.what()<< '\n';
	}

}

-----------------------------------------------------------------------------------------------------

YUKARIDAKİ DURUMDA NE YAPILABİLİR?

Func içinde try block yazılabilir ama bu exception yukarı çıkmamalı.
Biz bir hatayı yakalayıp bir değişkende tutup o hatanın tekrar gönderilmesini
sağlayabiliriz. 40. Ders Advanced exception handling

ÖR:
void func()
{
	std::cout << "func cagrildi\n";
	try{
		throw std::runtime_error{"error from func\n"};
	}
	catch(const std::exception&ex)
	{
		std::cout << ex.what();
	}
}

int main()
{
	set_terminate(my_terminate);
	try{
		std::thread t{func}; // Burada hata olursa artık yakalandı.
		t.join();
	}
	catch(const std::exception &ex)
	{
		std::cout << ex.what()<< '\n';
	}

}

-----------------------------------------------------------------------------------------------------

ÖNEMLİ ÖRNEK

#include <iostream>
#include <stdexcept>
#include <thread>

std::exception_ptr exptr = nullptr;  

void func(int x)
{
	std::cout << "func(int x) cagrildi x = " << x << '\n';
	try
	{
		if(x % 2 == 0)
			throw std::invalid_argument{"gecersiz arguman"};
	}
	catch(...)
	{
		exptr = std::current_exception(); // yakalanmış hata nesnesinin saklanmasını sağlıyor.
	}

	std::cout << "func(int x) sona erdi x = " << x << '\n'; 
}

int main()
{
	std::thread t{func,10};
	t.join();

	try{
		if(exptr)	// Yukarıda saklanmış olan hataya burada operator bool ile bakılıyor
		{			// Hata set edildiyse bloğa girer, set edilmediyse zaten nullptr olacağından girmez.

			std::rethrow_exception(exptr); // hata varsa rethrow ediliyor.
		}
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << '\n';
	}

}

ÇIKTI
-----
func(int x) cagrildi x = 10
func(int x) sona erdi x = 10
gecersiz arguman



-----------------------------------------------------------------------------------------------------

BENZER ÖRNEK DAHA

Kullanacağımız feature promise sınıflarından bunlar otomatik olarak yapılıyor.
Bizim oluşturmamız gerekmiyor.Kendimiz oluşturacaksak yolu bu.

#include <iostream>
#include <stdexcept>
#include <thread>

std::exception_ptr eptr{nullptr};

void foo()
{
	try{
		throw std::runtime_error{"error from foo\n"};
	}
	catch(...)
	{
		eptr = std::current_exception();
	}
}

int main()
{
	std::thread t{foo};
	j.join();
	
	try
	{
		if(eptr)
		{
			std::cout << "exception gonderilmis\n";
			std::rethrow_exception(eptr); 
		}
		else
		{
			std::cout << "exception gonderilmemis\n";
		}
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() <<'\n';
	}
}

-----------------------------------------------------------------------------------------------------

ÖR:

std::vector<std::exception_ptr> g_ex_vec;

std::mutex g_mutex;

void f1()
{
	throw std::exception{"exception from f1"};
}

void f2()
{
	throw std::exception{"exception from f2"};
}

void th_func1()
{
	try{
		f1();
	}
	catch(...)
	{
		std::lock_guard<std::mutex> guard{g_mutex};
		g_ex_vec.push_back{std::current_exception()};
	}
}
void th_func2()
{
	try{
		f2();
	}
	catch(...)
	{
		std::lock_guard<std::mutex> guard{g_mutex};
		g_ex_vec.push_back{std::current_exception()};
	}
}

int main()
{
	std::thread t1(th_func1);
	std::thread t2(th_func2);

	t1.join();
	t2.join();

	for(const auto &ex : g_ex_vec)
	{
		try{
			if(ex != nullptr) // exceptionlar push edilmişse nullptr olmayacak
			{
				std::rethrow_exception(ex); // rethrow edilecek
			}
		}
		catch(const std::exception &ex)
		{
			std::cout << ex.what() << '\n'; // Burada da yazılacak hatalar.
		}
	}
}

Exceptionlar nullptr değilse rethrow ediliyor ve yakalanıp yazdırılıyor.

-----------------------------------------------------------------------------------------------------

C++20 den önce ostream nesnesinin tamamen senkronize edilmiş hali yoktu.
Sadece tek karakter bazında senkronizasyon vardı. UB değil ama couta bir yazı gönderilse
yazının tamamı çıkmak zorunda değil. Her threadden bir karakter yazabilir.

void f1()
{
	for(int i = 0; i < 10000; ++i)
	{
		std::cout << "necati ergin" << "haydar";
	}
}

void f2()
{
	for(int i = 0; i < 10000; ++i)
	{
		std::cout << "mehmet ozuag" << "burak";
	}
}

void f3()
{
	for(int i = 0; i < 10000; ++i)
	{
		std::cout << "dogukan yigit polat";
	}
}


int main()
{
	std::thread t1{f1};
	std::thread t2{f2};
	std::thread t3{f3};

	t1.join();
	t2.join();
	t3.join();

}

Karman çorman bir çıktı:D

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------


SYNCSTREAM
----------
C++20 ile geldi. Yukarıdaki sorunu çözüyor.

std::osyncstream os{std::cout };  olarak kullanıldı.Ctoruna kendi ostream nesnemizi argüman oalrak veriyoruz.

-----------------------------------------------------------------------------------------------------

ÖR:

#include <syncstream>

void func()
{
	using namespace std::literals;
	
	std::this_thread::sleep_for(2000ms);
	
	std::osyncstream os{cout} ;

	for(int i = 0; i < 10000; ++i)
	{
		//std::cout << 13 << " alican " << 123.43 << " mustafa\n"; // Böyle yazarsak bozuluyor
		
		os << 13 << " alican " << 123.43 << " mustafa\n";
	}

}

void foo()
{
	using namespace std::literals;
	
	std::this_thread::sleep_for(2000ms);
	
	for(int i = 0; i < 10000; ++i)
	{
		//std::cout << 55 << " necati " << 35.52 << "ergin\n"; // Böyle yazarsak bozuluyor

		std::osyncstream{cout} << 55 << " necati " << 35.52 << "ergin\n";
	}
}

int main()
{
	std::osyncstream os{std::cout };

	std::thread t1{func};
	std::thread t2{foo};

	t1.join();
	t2.join();

}

osyncstream ile yazdırınca hiçbir bozulma yok.

-----------------------------------------------------------------------------------------------------

ÖR: Thread nesnelerini vector veya dizide tutuyoruz.

void cprint(char c)
{
	using namespace std::literals;
	for(int i = 0; i < 1000; ++i)
	{
		std::cout << c;
		std::this_thread::sleep_for(50ms);
	}
}

int main()
{
	//std:thread ta[10]; dizide olabilirdi

	std::vector<std::thread> ta[26];

	for(int i{}; auto &t : ta) // range      base for loop içinde değişken tanımlamakta yeni geldi.
	{
		// t = std::thread{cprint,'A' + i++}; dizi olarak yapsaydık böyle yapardık
	
		ta.push_back(std::thread{cprint,i + 'A'});
	}

	for(auto &t : ta)
	{
		t.join();
	}
}

-----------------------------------------------------------------------------------------------------

Basit vector örneği yapıldı onu yazmıyorum.
pushbackin hem lvalue ref hemde rvalue ref parametreli overloadu var.
emplace funcları ise argümanları funca perfect forward ediyor.

ÖR:

int main()
{
	std::ifstream ifs{"async01_txt"};
	if(!ifs)
	{
		std::cerr << "cannot open file\n";
		return 1;
	}

	std::vector<std::string> svec;
	std::string sline;

	while(std::getline(ifs,sline))
	{
		//svec.push_back(sline); // burada parametresi const string& olan çağrılır

		svec.push_back(std::move(sline)); // Burada da parametresi rvalue ref olan push back çağrılır.
										  // Moved from state teki nesne geçersiz değil.Sadece moved fromda
										  // tekrar atama yaparak kullanabilirim.Moved from state unspecified behavior
										  // string boş olabilir yani moved from stateteki nesne.
	}
	
}

-----------------------------------------------------------------------------------------------------

Bir önceki dersin sonunda yazılan örnek

#include <iostream>
#include <thread>

using namespace std;

std::thread::id main_thread_id;

void func()
{
	auto id = std::this_thread::get_id() << '\n';

	if(id == main_thread_id)
	{
		std::cout << "Func ana thread tarafından calistiriliyor\n";
	}
	else
	{
		std:cout << "Func child thread tarafından çalıştırılıyor\n";
	}
}

int main()
{
	main_thread_id = this_thread::get_id();

	func(); // bu anathread tarafından çalıştırılıyor yazar

	thread tx{func};

	tx.join(); // func child thread tarafından çalıştırılıyor.
}

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

YIELD
-----
Beni çalıştırma demek :D
bu func bir thread tarafından çalıştırıldığında, o func çalışmayacak ve çizelge dışında bırakılacak.

std::atomic<bool> ga_start{false};  //Senkronizasyon amaçlı atomic değişken kullanıldı.Sonra görülecek.

void func(char id)
{
	using namespace std::chrono;

	while(!ga_start)
	{
		std::this_thread::yield(); // bu threadin kodu çalıştırılmıyor burada.Önceliği diğerlerine veriyor.
	}

	std::this_thread::sleep_for(500ms);
	std::cout << id;
}

int main()
{
	std::thread ar_t[26];

	for(char i{'A'}; auto &t : ar_t)
	{
		t = std::thread(func, i++);
	}

	ga_start = true;
	
	for(auto &t : ar_t)
	{
		j.join();
	}
}

-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------

THREAD-LOCAL STORAGE DURATION

Memory model C++11 ile geldi.Daha önce C++ abstract machine sanki tamamen single thread gibi ele alınıyordu.
Memory model eklenmesiyle yani multithread programlamanın standart araçlarla yapılmasıyla birlikte
storage duration kategorilerine 1 tane daha eklendi.

automatic storage duration
dynamic storage duration
static storage duration

thread-local storage duration C++11 ile geldi.
keyword thread_local.

Bu tıpkı static ömür gibi ama static ömürlü bir nesne uygulamada tek.
thread local storage durationdaki nesne her thread başına bir tane.5 tane thread varsa thread-local
nesnesinden 5 tane olacak. Her threadin ulaştığı thread local nesne o threade özgü.
Bu threade özgü static ömürlü değişken(yada global değişken) gibi adeta.

Öyle durumlar varki her thread için ayrı değişken kullanmak istiyoruz. Mesela rasgele sayı
üreticisi için 10 thread varsa hepsi için seed deeğerini farklı olmasını istiyoruz.
Static ömürlü nesneyi paylaşımlı kullanırsa, 2 problem var
1 - Senkronize etmek gerekecek
2 - Hepsi aynı nesneyi kullanacak

Dolayısıyla 1 tane random random generator varsa onun üreteceği sequence baştan belli.
Her threadde ayrı sequence istersek bunların seed değeride farklı olmalı.

Özü şu, her thread için static ömürlü nesneye ihtiyaç varsa, fakat o thread sonlanınca o nesneye
ihtiyaç yok ise, senkronizasyon problemide olmadığından ideali thread-local nesne kullanmak.

NOT : Local scopeta tanımlanırken başında static olsada olmasada anlamı aynı.

-------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <thread>

thread_local int tval{0}; // main dahil her threadde bundan bir tane olacak

void func(const std::string& thread_name)
{
	++tval; // senkronizasyon gerekmiyor.

	std::osyncstream{std::cout} << " tval in thread " << thread_name << " is " << tval << "\n";
}

int main()
{
	std::thread tx(func,"a");
	std::thread ty(func,"b");

	{
		std::osyncstream{std::cout } << "tval in main : "<< tval << '\n'; // 0
	}

	tx.join(); // 1
	ty.join(); // 1

}

Çıktıların sırası belli değil.

-------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <string>
#include <thread>

thread_local int ival = 0;

void thread_func(int *p)
{
	*p = 42;
	std::cout << "ival = " << ival << '\n';  // 0 yazar. Burada yazdırılan main threaddeki ival değil.
											 // her threadde bir tane olan ival.
											 // *p yazdırılmıyor ival yazdırılıyor.
}

int main()
{
	std::cout << "ival = " << ival << "\n"; // 0
	ival = 9;

	std::cout << "ival = " << ival << '\n'; // 9
	
	std::thread t(thread_func, &ival); //  0.  yazdırdığımız *p değil o threadin ivali
	t.join();
	
	std::cout << "ival = " << ival << "\n"; // 42   // Ana threadin ivali bu

}


0
9
0
42

-------------------------------------------------------------------------------------------------------

ÖRNEK

Mutex yine senkronizasyon için var.

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

std::mutex mtx;

void func(int id)
{
	int x = 0;	// automatic ömürlü yerel değişken

	static int y = 0;   // static ömürlü yerel değişken. Bunlar shared resource.Birden fazla thread
						// bunlara erişirse aynı varlığa erişir. Bu durumda data race olmaması için
						// senkronizasyon işin içine girecek.Ortak resource
	
	thread_local int z = 0;    // Her thread başına tek.
							   // static koysakta koymasakta sentaks hatası yok. İkiside anlam olarak aynı
								

	++x;
	++z;
	std::lock_guard guard(mtx);
	++y;

	std::cout << "thread id : " << id << "    x (automatic storage)  = " << x << "\n";
	std::cout << "thread id : " << id << "    y (automatic storage)  = " << y << "\n";
	std::cout << "thread id : " << id << "    z (automatic storage)  = " << z << "\n\n";
}

void foo(int id)
{
	func(id);
	func(id);
	func(id);
}

int main()
{
	using namespace std;
	
	thread tx{foo,0};
	thread ty{foo,1};
	thread tz{foo,2};

	tx.join();
	ty.join();
	tz.join();
}

ÇIKTI ÇOK ÖNEMLİ !!!!!
----------------------
x ler otomatik ömürlü
y ler Static ömürlü. ortak resource. 
z ler thread başına bir tane.

thread id : 1    x (automatic storage)  = 1
thread id : 1    y (automatic storage)  = 1
thread id : 1    z (automatic storage)  = 1

thread id : 1    x (automatic storage)  = 1
thread id : 1    y (automatic storage)  = 2
thread id : 1    z (automatic storage)  = 2

thread id : 1    x (automatic storage)  = 1
thread id : 1    y (automatic storage)  = 3
thread id : 1    z (automatic storage)  = 3

thread id : 0    x (automatic storage)  = 1
thread id : 0    y (automatic storage)  = 4
thread id : 0    z (automatic storage)  = 1

thread id : 0    x (automatic storage)  = 1
thread id : 0    y (automatic storage)  = 5
thread id : 0    z (automatic storage)  = 2

thread id : 0    x (automatic storage)  = 1
thread id : 0    y (automatic storage)  = 6
thread id : 0    z (automatic storage)  = 3

thread id : 2    x (automatic storage)  = 1
thread id : 2    y (automatic storage)  = 7
thread id : 2    z (automatic storage)  = 1

thread id : 2    x (automatic storage)  = 1
thread id : 2    y (automatic storage)  = 8
thread id : 2    z (automatic storage)  = 2

thread id : 2    x (automatic storage)  = 1
thread id : 2    y (automatic storage)  = 9
thread id : 2    z (automatic storage)  = 3


Otomatik ömürlü her her callda baştan çağrılıyor. değeri 0 dan 1 e çıkıyor.
Static ömürlü olan hangi thread olursa olsun 1 artıyor değeri çünkü ortak resource
Thread local ise sadece belirli thread için artırıyor. 

MOLA

-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------

Biraz daha basit bir örnek

#include <iostream>
#include <syncstream>
#include <mutex>

thread_local int gt{};

void func(char c)
{
	++gt;
	std::osyncstream{std::cout} << c << " " << gt << '\n';
}


int main()
{
	using namespace std;

	vector<thread> tvec;

	for(char c = 'a'; c <= 'z'; ++c)
	{
		tvec.emplace_back(func,c);
	}

	//

	for(auto &t : tvec)
	{
		t.join();
	}
}

Sıralı değil  !!!

b 1
i 1
h 1
.
.
.
n 1
p 1
z 1

-------------------------------------------------------------------------------------------------------

Şimdi senkronizasyon oluşturalım.

#include <iostream>
#include <syncstream>
#include <mutex>

int gt{};
std::mutex m;

void func(char c)
{
	std::lock_guard<std::mutex> myguard{m}; // bu satır birsonraki ders görülecek.
	++gt;
	std::osyncstream{std::cout} << c << " " << gt << '\n';
}


int main()
{
	using namespace std;

	vector<thread> tvec;

	for(char c = 'a'; c <= 'z'; ++c)
	{
		tvec.emplace_back(func,c);
	}

	//

	for(auto &t : tvec)
	{
		t.join();
	}
}

ÇIKTI BU SEFER SIRALI(Release modda çalıştırdık)
---------------------
a 1
b 2
c 3
d 4
.
.
.
w 23
x 24
y 25
z 26


-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------

BIZ THREADLERI PARALEL PROGRAMLAMA IÇINDE YAPIYORUZ.DAHA ÖNCE YAZMIŞTIK.
Simaultaneously

std::thread::hardware_concurrency ---> bu desteklenen core sayısını veriyor
Buradaki threadler ayrı ayrı corelarda çalışacak diye birşey yok. 
Bende 24 çıktı Logical processor sayısı.24 threade kadar donanımsal olarak destekleme şansı var.
24 threadde farklı farklı core larda çalışacak diye bir garanti yok.


ÖR:

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdint>

uint64_t sum_odd = 0ull;
uint64_t sum_even = 0ull;
constexpr uint64_t n = 1'000'000'000;

void get_sum_odds()
{
	for(uint64_t i = 1; i < n; i += 2)
		sum_odd+= i;
}

void get_sum_evens()
{
	for(uint64_t i = 0; i <= n; i += 2)
		sum_odd+= i;
}


int main()
{
	using namespace std::chrono;

	auto start = steady_clock::now();
	get_sum_evens();
	get_sum_odds();
	auto end = steady_clock::now();

	std::cout << "hesaplama tamamlandi toplam süre : " << duration_cast<milliseconds>(end-start).count() << '\n'; 
	std::cout << "teklerin toplami : " << sum_odd << '\n';
	std::cout << "ciftlerin toplami : " << sum_even << '\n';
}

hesaplama tamamlandi toplam sure : 222
teklerin toplami : 250000000000000000
ciftlerin toplami : 250000000500000000



Burada ise ayni örneği 2 thread halinde yaptik.

int main()
{
	using namespace std::chrono;

	auto start = steady_clock::now();
	std::thread t1{get_sum_evens};
	std::thread t2{get_sum_odds};
	t1.join();
	t2.join();
	auto end = steady_clock::now();

	std::cout << "hesaplama tamamlandi toplam süre : " << duration_cast<milliseconds>(end-start).count() << '\n';
	std::cout << "teklerin toplami : " << sum_odd << '\n';
	std::cout << "ciftlerin toplami : " << sum_even << '\n';
}

Süre yarıya indi neredeyse
hesaplama tamamlandi toplam sure : 120
teklerin toplami : 250000000000000000
ciftlerin toplami : 250000000500000000

Hoca birde farkı daha net görmek için bir bekleme koydu functionların içinde
std::this_thread::sleep_for(4ms) girdi hatta.constexpr sayıyıda 1000 yaptı ....

-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------

NATIVE HANDLER

Os in kendi threadi için bir handle veriyor. Bu bir adres. OS in kendi apilerinde kullanılabilir.
thread id nin bunla alakası yok. joinable değilse thread id tüm threadler için aynı olmak zorunda
genelde 0 oluyor.

int main()
{
	std::thread t{get_sum_events};

	std::cout << t.native_handle(); // 000000000000008C

	t.join();
}


int main()
{
	std::thread t1;
	std::thread t2;
	std::thread t3;

	std::cout << t1.get_id() << '\n'; // 0
	std::cout << t2.get_id() << '\n'; // 0
	std::cout << t3.get_id() << '\n'; // 0
}



Workload verseydik

void func() {}
void foo() {}
void bar() {}

int main()
{
	std::thread t1{func};
	std::thread t2{foo};
	std::thread t3{bar};

	std::cout << t1.get_id() << '\n'; // 23420
	std::cout << t2.get_id() << '\n'; // 22208
	std::cout << t3.get_id() << '\n'; // 11536

	t1.join();
	t2.join();
	t3.join();
}

-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------


THREAD TARAFINDAN ÇALIŞTIRILACAK FUNCTIONLARIN RETURN DEĞERLERİNİN ALINMASI VE THREADLERİN
PARALEL ÇALIŞTIRILMASI

Minimal olarak paralel pgoramlama yapacak programcıların ihtiyacını en yüksek seviyede karşılamak için
threadlerin paralel çalıştırılmasını sağlayan üstelik çalıştırılan funcın return değerini o threadi
oluşturan koda iletme mekanizmasında kapsayan yüksek seviyeli arayüz var. Bu bir function şablonu.
Adı async.

Amaç birden fazla threadi paralel çalıştırmaksa ve özellikle çalıştıracağımız threadden bir outcome elde edeceksek
(outcome derken function return değeri ve exception throw etme durumlarını kapsıyor)kullanıyoruz.
Yani istediğimiz kadar thread çalıştırabileceğimiz eşzamanlı olarak çalıştırabileceğimiz ve geri dönüş değerlerini
aldığım gibi exception gönderilmesi durumunda o exceptionuda işleyebileceğiz. 

Normalde daha düşük seviyede araçlarla da yapabiliriz. Thread nesnelerini kendimiz oluşturabiliriz mesela.
Bu bir functiondan thread tarafından çalıştırılan taskten return değerinin aktarılması için sınıf
nesnelerini kendimiz oluşturabiliriz( feature ve promise sınıfları). Ama basit görevler için yüksek seviyeli interface kullanılacak.

async basit bir func.
future header file içinde

Çalıştırılan threadden, onu çalıştıran threade return değerinin aktarımı için shared state e ihtiyaç var.
Async kullanmazsak bunu oluşturma görevide bizde. Feature nesnesi türünden bir nesne oluşturmak gerekecek.
Async hazır veriyor. Async kullanmasak shared stati oluşturma görevi bizde olacak. Feature türünden bir nesne
olutşurmak gerekecek ve bu nesneyi threadler arası iletişimde kullanmamız gerekecekti. Asyncte bu yok
herşey hazır

-------------------------------------------------------------------------------------------------------

Aşağıdaki gibi kullanılıyor.Tabi birsürü template var.
async(Function&&f, Args&&...args );

Funcın return değeri int ise async return değeri std::feature<int>
ağer double i se return değeri std::feature<double>

Async kullanarak 3 farklı olanaktan yararlanabiliriz.
1 - Ben bu callable ın asenkron çalışmasını istiyorum yani ayrı bir thread olarak çalışmasını istiyorum.
2 - Ben bu callable ın ben istediğimde senkron çalışmasını istiyorum
3 - Bu kararı derleyicinin duruma göre karar vermesini sağlayabiliyoruz(Derleyiciler pek uymuyor buna)

Eğer biz aşağıdaki parametreyi kullanırsak(std::launch policy) seçimimizi açıkça kendimiz söylemiş oluyoruz.
async(std::launch policy, Function &&f, Args&&... args);

std::launch bir enum class. Bu enum class türünün enumeratorleriyle belirlenmiş.
Eğer std::lauch::async argüman olarak gönderirsek asenkron çalıştır diyoruz.
Eğer std::lauch::deferred argüman olarak gönderirsek lazy evaluation olsun diyoruz.
Lazy evaluation demek, biz bu görevi verdik ama bir funcı çağırıncaya kadar fiilen kod çalıştırılmayacak.
Ne zamanki outcome a(çalıştırılacak callable dan gelecek return değerine) ihtiyaç duyduk o zaman
func kodu çağrılacak. 

Bunlar bitwise operationsa açık bir enum türü. Bu sebeple bunları bitsel veya ile birleştirirsek
(std::lanuch::async | std::lauch::deferred) bu durumda kararı derleyiciye bırakmış oluyoruz.
std::launch parametresinin hiç olmadığı overloadda böyle çalışıyor.

Yani 
async(Function&&f, Args&&...args );		bu aslında aşağıdaki gibi
async(std::lanuch::async | std::lauch::deferred, Function&&f, Args&&...args );

-------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <thread>
#include <future>  // async bunun içinde

int foo(int x)
{
	std::cout << "foo cagrildi x = " << x << "\n";

	// Burada exception gönderseydik bunu yakalayıp exception_ptr değişkeninde tutardık
	// ana kodda da exception var mı yok mu diye bakardık. exception var mı yok mu anlardık.
	// ... ders başında yapıldı bunlar

	return x * x;
}

Normalde böyle yapardık.
int main()
{
	std::thread t{foo,10};

	j.join(); // return değeri ile hiç iletilmedi.
}


Async kullanalım
int main()
{
	auto ft1 = std::async(foo,10); // bunu bu şekilde çağırmak ile aşağıdaki ayını anlamda
									// return değeri std::feature callable ın return değeri türü
									// foo nun return değeri int oldupundan, feature<int> olacak eturn değeri türü

	// Auto type deduction olmasaydı aşağıdaki gibi yazardık
	// std::future<int> ft2 = std::async(foo,10);


	// std::async(std::launch::async | std::launch::deferred ,foo,10); 
	// aynı yukarıdaki ile
	// uzun uzun yazmak yerine diğer overlodu kullanıyoruz.
	// return değeri std::future<int> türünden
	

	auto val = ft.get(); // bu noktaya kadar func çalışıp bittiyse func return değerini alıyoruz
						 // eğer func çalışması başlamış ama devam ediyorsa joindeki gibi geti çağıran thread bloke oluyor 
						 // ve geti çağıran funcın kodunun bitmesi bekleniyor.

}

async olabilecek en yüksek seviyeli interface e sahip.

-------------------------------------------------------------------------------------------------------

GET

Örnek daha derli toplu halde
Asenkron çalıştıracağız

int foo(int x)
{
	std::cout << "foo cagrildi x = " << x << "\n";

	return x * x;
}

class Myclass{};

Myclass bar();

int main()
{
	auto ft = std::async(foo,5); async return değeri tutuldu
	// std::future<Myclass> ft = std::async(foo,5); // buda yazılabilir

	// FUNCTIONUN RETURN DEĞERİNİ GET İLE ALIRIZ
	ft.get();
	
	std::cout << "val = " << val << '\n';

}

GETİN ÇAĞRILDIĞI NOKTA İLE İLGİLİ ÇOK ÖNEMLİ AÇIKLAMA !!!!!!
1 - Geti çağırınca zaten bu funcın kodu çalışıp bitmişse burada hiçbir şekilde bloke olmuyor
	Funcın doğrudan return değerini alıyoruz.

2 - Eğer bu func çalışmaya başlamamışsa(yani deferred olarak çalışacaksa) karar derleyicide
	Kendi deferred olarak seçmişte olabilir. Yada biz deferred ilk argüman olarak yazabilirdik.
    Bu durumda geti çağırana kadar bu func çağrılmayacaktı ama geti çağırınca bu task olan func çağrılacaktı.
	std::async(std::launch::deferred,foo,5); şeklinde çağrılabilir

3 - Geti çağırdığımızda func kodu çalışmaya başlamış ama bitmemiş halen devam ediyor.
	Bu durumda threadde join çalıştırır gibi geti çağıran threadde bloke oluyor bu funcın kodunun
	çalışmasının bitmesi gerekiyor.

Özellikle funca async enumeratoru geçilirse(std::async(std::launch::async,foo,5);) bunu asenkron çalıştırıyoruz.
Eğer bunu asenkron çalıştıracak kaynak yoksa o zaman system error türünden exception throw ediyor. 
Dolayısı ile burada ya async ya asenkron çalışacak ya da exception throw edecek.

-------------------------------------------------------------------------------------------------------

ÖR:

int foo(int x)
{
	std::cout << "foo cagrildi x = " << x << "\n";

	return x * x;
}

void func()
{

}

class Myclass{};

Myclass bar();

int main()
{
	auto ft = std::async(std::launch::async,foo,5);
	
	func(); // programın akışı bu noktadan sonra her iki threadde paralel çalışıyor.
			// hem foo hemde func kodu çalışıyor.

	std::cout << "val = " << val << '\n';

}

-------------------------------------------------------------------------------------------------------

ÖR:

int f1()
{
	///...
	return 1;
}

int f2()
{
	///...
	return 2;
}

int f3()
{
	///...
	return 3;
}

class Myclass{ };


int main()
{
	auto ft1 = std::async(std::launch::async, f1);
	auto ft2 = std::async(std::launch::async, f2);
	auto ft3 = std::async(std::launch::async, f3);
	
	std::cout << ft1.get() << '\n';
	std::cout << ft2.get() << '\n';
	std::cout << ft3.get() << '\n';

}

NOT : SONUÇ HESAPLANMAMIŞSA GET THREADI BLOKE EDIYOR. !!!!!!!!!!!!!!!!!!!!!!
Asenkron çalışıyordur ama çalışması tamamlanmamıştır o zaman get çağrılırsa çalışması bitene kadar beklenecek
Deferred çalışyordur, burada zaten geti çağırana kadar zaten function çağrılmayacak.

NOT : Return değeri olan feature nesnesinin bir validity si var. Yani bir feature nesnesi bir t zamanında
valid veya invalid olabilir. get func ını sadece 1 kere çağırabiliriz. get çağrıldıktan sonra tekrar get çağrılırsa
bu undefined behavior. Standartlar burada exception throw etme garantisi yok ama exception throw etmeye özendiriyor
diye bir ifade kullanıyor. Yani invalid durumdaki feature nesnesinin get funcını çağırmak undefined behavior ama
genelde tüm derleyiciler feature error türünden exception throw ediyor. Get 1 kere çağrılacak.

-------------------------------------------------------------------------------------------------------

#include <stdexcept>
#include <future>
#include <iostream>

int func()
{
	throw std::runtime_error{"error from func\n"}; // yorum yapıp dene tekrar
	return 1;
}


int main()
{
	try{
		auto ftp = async(std::launch::async, func); // ftr nin türü std::feature<int>
		auto val = ftr.get(); // ftr nin değeri funcın return değeri. Exception gönderseydi yakalayacağız zaten
		std::cout << "return value is : " << val << '\n';
	}
	catch(const std::exception& ex)
	{
		std:cout << "exception caught : " << ex.what() << '\n';
	}
}


-------------------------------------------------------------------------------------------------------

#include <stdexcept>
#include <future>
#include <iostream>

int func()
{}

int func()
{}

int main()
{
	// auto x = func() + foo(); parallel olarak çalıştıracağız bu işi

	auto ft_foo = async(std::launch::async, foo);
	auto func_val = func(); // buda main threadden çalışıyor.

	auto x = func_val + ft_foo.get(); // get çağrıldığında işlem bitmemişse bloke olacak.
									  // Bitmişse zaten değeri alacağız.Bunlar paralel çalışmış olacak.
}

-------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <algorithm>

std::string get_str_letters(int n)
{
	using namespace std::literals;

	std::string s(n, '\0');

	std::generate_n(s.begin(), n, []{ return rand() % 26 + 'A';});
	std::this_thread::sleep_for(3s);
	
	return s;
}

std::string get_str_digits(int n)
{
	using namespace std::literals;

	std::string s(n, '\0');

	std::generate_n(s.begin(), n, []{ return rand() % 10 + '0';});
	std::this_thread::sleep_for(3s);

	return s;
}


Asenkron olarak çalıştırıyoruz. Şimdi asenkron çalışacak. 
Süre aşağıdakinin neredeyse yarısı kadar olacak. 3 saniye civarı
Bunlar ayrı threadler tarafından çalıştırıldı.yani asenkron
int main()
{
	auto start = std::chrono::steady_clock::now();

	auto ftr1 = async(std::launch::async, get_str_letters,20);
	auto ftr2 = async(std::launch::async, get_str_digits,20);

	auto s1 = ftr1.get(); 
	auto s1 = ftr2.get(); 
	
	auto end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<double>(end-start).count() << " saniye\n";

	std::cout << s1 + s2 << "\n";

}


Deferred olarak çalıştırıyoruz.Burada birinci func çağrılacak bloke olacak sonra ikinci func çağrılacak
süre toplam süreye eşit. 6 saniye civarı

int main()
{
	auto start = std::chrono::steady_clock::now();

	auto ftr1 = async(std::launch::deferred, get_str_letters,20);
	auto ftr2 = async(std::launch::deferred, get_str_digits,20);

	auto s1 = ftr1.get(); // Buraya kadar çalıştırılan bir func yok.get funcı çalıştığı anda func kodu çalıştırılacak
						  // return değeri olan future<std::string> nesnesini alacağız.
	
	auto s1 = ftr2.get(); // Burada da diğeri çalışacak

	auto end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration<double>(end-start).count() << " saniye\n";

	std::cout << s1 + s2 << "\n";

}

-------------------------------------------------------------------------------------------------------

ÇOK ÇOK ÖNEMLİ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Aşağıdaki gibi çalıştırırsak ne olur?

int main()
{
	async(std::launch::async, get_str_letters,20);
	async(std::launch::async, get_str_digits,20);
}

BU DURUMDA HIÇBIR KAZANÇ YOK. FUNC GERI DÖNÜŞ DEĞERI OLAN NESNENIN ASENKRON ÇALIŞTIRDIĞIMIZDA
HAYATI BITTIĞINDE OTOMATIK OLARAK GET FUNCI ÇAĞRILIYOR. YANI FEATURE NESNESI DESTRUCT OLDUPUNDA
GETI FIILEN ÇAĞIRMASAKTA GET ÇAĞRILIYOR. 1. FUNC ÇAĞRILDIĞINDA RETURN DEĞERINI KULLANMADIĞIMIZ IÇIN
ORADAKI GEÇICI NESNENIN HAYATI BITECEK VE BU DURUMDA GET FUNCI ÇAĞRIACAK. GET FUNCI ÇAĞRILDIĞI IÇIN 1. BLOKE OLACAK
1. BITTIKTEN SONRA 2. BAŞLAYACAK. YANI TOPLAM SÜRE 3 DEĞIL 6 SANIYE OLACAK. FUNC RETURN DEĞERI OLMASADA 
ASENKRON ÇAĞIRMAYI SAĞLAMAK IÇIN GETI ÇAĞIRMAK GEREKIYOR. GET ÇAĞRILMAZSA FEATURE NESNESININ HAYATI BITER VE
OTOMATIK OLARAK GET FUNCI ÇAĞRILIR.GET TE ONU BLOKE EDIYOR. YUKARISI 6 SANİYE CİVARI SÜRER.

-------------------------------------------------------------------------------------------------------

Burada get çağrılmasa ne olurdu ? 

int main()
{
	auto f1 = async(std::launch::async, get_str_letters,20);
	auto f2 = async(std::launch::async, get_str_digits,20);
}

NESNELERIN HAYATI BITTIĞINDE F1 VE F2 DESTROY OLDUĞUNDA BUNLARIN GET FUNCLARI ÇAĞRILACAK.
YANI GETLERIN ÇAĞRILMASI MAIN BITTIĞINDE ÇAĞRILACAK.YINE GET ÇAĞRILACAK
ARADA FARK, RETURN DEĞERINI BIR FEATURE NESNESINE BAĞLAMAK.


*/
