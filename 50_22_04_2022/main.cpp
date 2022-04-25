/*

Async funcını inceliyorduk.Buradan promise ve future sınıflarına geçeceğiz.

async bize future sınıfı türünden bir nesne veriyor. Future gelecekte elde edilecek değeri 
tutacak nesne. Bu bir sınıf şablonu. Asynce callable olarak hangi func verilirse async return değeri
o callable ın future açılımı(future<int> gibi). Biz asyncten future nesnesini aldığımız zaman
onun get funcını çağırdığımızda return değerini elde ediyoruz.

Asynci 3 farklı şekilde çalıştırabiliyorduk.
1 - Birinci parametreye launch policy geçmiyoruz. Bu launch policy yi compilera bırakıyoruz demek.
	Kararı runtime verecek.
2 - İllaha asenkron çalıştıracaksam launch::async verilecek yada deferred çalıştırılacaksa
	launch::deferred geçilir.
3 - Birinci parametreye launch::deferred | launch::async geçmek ile argüman göndermemek aynı.
	1. adım gibi çalışır.

----------------------------------------------------------------------------------------------------

ÖR:

#include <chrono>
#include <future>
#include <iostream>


int main()
{
	auto tp_start = std::chrono::system_clock::now();

	auto lazy_async = std::async(std::launch::deferred, [] {return std::chrono::system_clock::now(); });
	auto eager_async = std::async(std::launch::async, [] {return std::chrono::system_clock::now(); });

	std::this_thread::sleep_for(std::chrono::seconds(1));

	using dsec = std::chrono::duration<double>; // yazma kolaylığı, duration<double,1> demek

	auto deferred_sec = static_cast<dsec>(lazy_async.get() - tp_start).count();
	auto eager_sec = static_cast<dsec>(eager_async.get() - tp_start).count();

	std::cout << "duration for deferred in sec : " << deferred_sec << '\n';
	std::cout << "duration for eager in sec : " << eager_sec << '\n';
}

Output
------
duration for deferred in sec : 1.00487 // Deferred olan 1 saniye sonra çalışan
duration for eager in sec : 0.0002979  // bu ise zaten asenkron olarak çalıştı

launch::async yerine hiçbirşey yazmazsak compiler kendi karar verecek. Yine asenkron çalıştı
launch::async | launch::deferred yazarsak yine asenkron çalıştı

----------------------------------------------------------------------------------------------------

ÖR: İkiside asenkron olursa

#include <chrono>
#include <future>
#include <iostream>


int main()
{
	auto tp_start = std::chrono::system_clock::now();

	auto lazy_async = std::async(std::launch::async, [] {return std::chrono::system_clock::now(); });
	auto eager_async = std::async(std::launch::async, [] {return std::chrono::system_clock::now(); });

	std::this_thread::sleep_for(std::chrono::seconds(1));

	using dsec = std::chrono::duration<double>; // yazma kolaylığı, duration<double,1> demek

	auto deferred_sec = static_cast<dsec>(lazy_async.get() - tp_start).count();
	auto eager_sec = static_cast<dsec>(eager_async.get() - tp_start).count();

	std::cout << "duration for deferred in sec : " << deferred_sec << '\n';
	std::cout << "duration for eager in sec : " << eager_sec << '\n';
}

Output
------
duration for deferred in sec : 0.0003844
duration for eager in sec : 0.0005767

Get yapana kadar zaten sonuçlanmışlardı.


----------------------------------------------------------------------------------------------------

#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <future>

std::map<char, size_t> histogram(const std::string& s)
{
	std::map<char, std::size_t> cmap;

	for (char c : s)
	{
		++cmap[c];
	}

	return cmap;
}

std::string get_sorted(std::string str)
{
	sort(begin(str), end(str));
	erase_if(str, [](char c) { return isspace(c); }); // erase remove idiyomu yapmaya gerek yok C++20
	return str;
}

bool is_vowel(char c) // sesli harf sorgusu
{
	static const char vowels[]{ "AEIOUaeiou" };
	return std::find(std::begin(vowels), std::end(vowels), c) != std::end(vowels);
}

size_t count_vowels(const std::string& s)
{
	return count_if(begin(s), end(s), is_vowel);
}

int main()
{
	std::string sline;
	std::cout << "enter a string: ";

	getline(std::cin, sline);

	auto hist = async(std::launch::async, histogram, sline);	// ayrı ayrı funclar. Burada simultaneously var
	auto sorted_str = async(std::launch::async, get_sorted, sline);
	auto vowel_count = async(std::launch::async, count_vowels, sline);

	for (const auto& [c, count] : hist.get())
	{
		std::cout << c << ": " << count << '\n';
	}

	std::cout << "sorted string : "
		<< quoted(sorted_str.get()) << '\n'
		<< "total vowels : "
		<< vowel_count.get() << '\n';

}



-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------

FUTURE VE PROMISE
-----------------
Bize future veren 3 tane yapı var.
1 - Promise bize bir future veriyor. Bir promise nesnesi oluşturduğumuzda ve
    onun get_future funcını çağırdığımızda bize o promise, shared state'i
    paylaşacak bir future nesnesi veriyor.Bu en alt seviyede olan

2 - async funcı bize bir future veriyor(arka planda yine promise kullanıyor ama 
    promise i bizden gizliyor)

3 - Package task'te future veriyor. get_future funcı ile elde ediliyor.

NOT : BIR FUTURE'IN OLMASI IÇIN ONUN ILIŞLKILENDIRILDIĞI BIR PROMISE NESNESI OLMALI.
      BU PROMISE I KENDIMIZ OLUŞTURACAĞIMIZ GIBI YUKARIDAKI YÖNTEMLERI UYGULAYARAK
      TA BUNU ELDE EDEBILIRIZ !!!!

BIR RESIM EKLEDIM REPOYA.

	PROMISE : SENDER			  FUTURE : RECEIVER
		|					 |
		|					 |
		|					 |
		|-------------->  CHANNEL -------------->|

Bu bir kereliğine kullanılacak bir iletişim/data kanalı. Ortada bir gönderen birde alan var.
Bu göndericinin ve alıcının beraber kullanacağı shared state var.
Promise gönderen tarafı temsil ediyor. Biz bir promise nesnesi oluşturunca, 
o promise nesnesi vasıtayla shared statei set ediyoruz.Burada 2 şansımız var,

1 - Value ile set etmek
2 - Exception ile set etmek.

Promise sınıfının set_value ve set_exception funcı var.Görülecek...
set_value ile bir değer set edilebilir, böylece future tarafı get funcı ile bu değeri alabiliyor.
yada exception throw etmek istrsek set_exception funcı çağrılır, bu durumda da future tarafı/alıcı taraf
o get funcını çağırınca exception throw ediyor ve o gönderilen exceptionu yakalayabiliyor.

Burada kritik nokta bunların promise/future ların ayrı threadler tarafından kullanılabilir.
Bu iletişim kanalı bir defalık  kullanılabilir. Promise shared statei bir kere set edebiliyor.
Future da bir kere get edebiliyor.

Promise shared statae i bir kere set edebiliyor, future da bir kere get edebiliyor.
2 side 2. kez get veya set yaparsa exception throw ediyor.

Sonradan eklenen shared_future sınıfını kullnarak bu kez birden fazla kez shared state
get edilebiliyor.

Async'in altında bu mekanizma var.Biz asenkron çalıştırmak istediğimiz bir koddan bir
outcome / result almak istiyorsak, bir şekilde değeri oluşturacak threadin bir promise
nesnesinin olması gerekiyor. Promise nesnesinide büyük ihtimal(başka olasılıklarda var)
biz funca argüman olarak geçeceğiz.Böylece o değeri üreten kod, promise nesnesini set edecek. 

PROMISE VEYA FUTURE SINIF ŞABLONU !!!

PROMISE NESNESI THREAD SINIFI GIBI, COPY YE KARŞI KAPALI AMA MOVE A KARŞI AÇIK!!!!!

----------------------------------------------------------------------------------

Header file future.Asyncte future içindeydi.
Bizim bir promise nesnesine ihtiyacımız var. 
Oluşturulacak outcome hangi türdense, feature/promise'in o tür açılımını kullanacağız

Kodu adımlara böldüm

#include <future>
#include <iostream>

int main()  // tek thread kullanacağız bu örnekte
{
	// 1
	std::promise<int> prom; // Bu nesneye sahip olan kod gelecekte bir int alacak diyor bu kod.
				// int almazsa exceptionda alabilir.
	
	//-----------------------------------------------------------------------------------
	
	// 2
	// Bu promiseten bir future almamız lazım. Promise ve future bir data paylaşımı yapıyor.
	std::future<int> ftr = prom.get_future();  // promise<string> olsaydı bu future<string> verecekti
						   // auto da kullnılabilirdi.

	//-----------------------------------------------------------------------------------
	
	// 3
	// Valid durumda ama değer hazır değil. Promise nesnesi shared state i set etmedi daha
	if(ftr.valid())
	{
		std::cout << "future nesnesi valid durumda\n"; // Yani bunun get funcı çağrılabilir
	}						       // 1 kere get funcı çağırınca artık invalid duruma gelir.
	else
	{
		std::cout << "future nesnesi invalid\n"; 
	}

	(void)getchar();

}	


DEVAM !!!!!!!!!!!!!!!!!
KOD DEĞİŞİYOR

#include <future>
#include <iostream>

int main()  // tek thread kullanacağız bu örnekte
{
	std::promise<int> prom;

	std::future<int> ftr = prom.get_future(); 

	prom.set_value(991); // Dikkat

	auto val = ftr.get(); // artık val hazır

	std::cout << "val = " << val << '\n'; // 991 

	if(ftr.valid())
	{
		std::cout << "future nesnesi valid durumda\n"; 
	}													
	else	// Get çağrıldığı için artık invalid durumda
	{
		std::cout << "future nesnesi invalid\n"; 
	}

	(void)getchar();
}	


DEVAM !!!!!!!!!!!!!!!!!
KOD BİRAZ DAHA DEĞİŞİYOR

#include <future>
#include <iostream>

int main()  // tek thread kullanacağız bu örnekte
{
	std::promise<int> prom;

	std::future<int> ftr = prom.get_future();

	prom.set_value(991);

	auto val = ftr.get();

	std::cout << "val = " << val << '\n';

	try{
		val = ftr.get();
	}
	catch(const future_error& ex)
	{
		std::cout << ex.what() <<'\n';
	}
}

---------------------------------------------------------------------------------

GET FUNCI ÇAĞIRDIK AMA DEĞER SET EDİLMEMİŞSE HENÜZ NE OLACAK

ÖR:

int main()
{
	std::promise<int> prom; 

	auto ftr = prom.get_future();

	auto val = ftr.get();

	std::cout << "val = " << val << '\n';

}

Burada program bekliyor. Set edilene kadar böyle kalacak.


NOT
SET EDILIP EDILMEDIĞINI ANLAMAK IÇIN AYRICA WAITFOR VE WAITUNTILL FUNCLARI KULLANILIR.
BU IKI FUNC ARASINDA TEK FARK BIRININ DURATION ALMASI, DIĞERININ ISE BIR TIMEPOINT ALMASI
BU FUNCLARIN RETURN DEĞERLERI ÖZEL BIR TÜR. STATUS TÜRÜ. BU BIR ENUMERATOR TÜRÜ.
BU READY,EXPIRED YADA DEFERRED OLABILIR. BU DEĞERLERE BAKARAK DURUM HAKKINDA BILGI ALIYORUZ.
READY ISE DEĞER HAZIR, EXPIRED İSE SÜRE GEÇMİŞ AMA DEĞER HAZIR DEĞİL, DEFERRED İSE ZATEN
DEĞER HAZIR DEĞİL GET ÇAĞRILDIĞINDA DEĞER ALINIR

---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

Async işimizi görürse async çağrılırsa daha iyi olur.Her zaman async işimizi görmeyebilir.

Bir funcın shared statei set etmesi için onun promise nesnesine ihtiyacı var.
Promise nesnesi
1 - Global olabilir. Biri global nesneyi set eder, diğeride o promise nesnesine erişip
	ondan future alır.

2 -	Değeri elde etmek isteyen kod, promise nesnesini kendisi gönderir. Bu copy semantics ile
	olamayacağı için ya rvalue reference parametre olmalı yada move semantics devreye girecek


ÖR:

//Bu şekilde sum square bir değer hesaplayacak ve bu değer ile promisi set ediyoruz.
//Böyölece bu promise in future tarafını kullnacağız yani get funcını çağırınca değeri
// elde edeceğiz.
void sum_square(std::promise<int>&& prom, int x, int y)
{
	prom.set_value(x * x + y * y);
}

// yukarıdaki gibi. Set value ile x / y yi verecek.
// Buradaki olay function return değeri ile bu değerler iletilmiyor. Asenkron çalıştırılıyor
// ve değer set_value ile set ediliyor.
struct Div{
	void operator()(std::promise<int>&& prom, int x, int y)const
	{
		prom.set_value(x/y);
	}
};


int main()
{
	int x{90},y{15};

	std::promise<int> sum_square_prom;
	std::promise<int> div_prom;



	std::future<int> fp_sumsquare = sum_square_prom.get_future();
	std::future<int> fp_div = div_prom.get_future();
	// burada auto da kullanılabilirdi.
	// Burada kullandığımız future nesneleri. Bu feature nesnelerini promise in get_feature funcına
	// çağrı yaparak almakla async funcının return değeri olarak almak arasında bir fark yok.
	// Async nasıl bir feature return ediyorsa, promise in get_feature funcıda bir feature veriyor.
	// diğer yöntemde package task. 3 yöntem var.



	// DİKKAT !!!!!!!!!!!!
	// Threade iş yükü olarak sumsquare funcı verildi. Funcınn 1. parametresi bir promise nesnesi
	// sağ taraf değerine dönüştürüp promise i gönderdik ve diğer argümanları gönderdik.
	std::thread tss(sum_square,std::move(sum_square_prom),x,y);  
	
	std::thread tdd(Div{}, std::move(div_prom),x,y); // buda bir üstteki gibi.




	std::cout << x << " * " << x << " + " 
			  <<  y  << " * " << y << " = " 
			  << fp_sumsquare.get() << std::endl; // Sonuç hazırsa get ile alacak
												  // değilse bloke edecek.
	
	std::cout << x << " / " << y << " = " << fp_div.get() << std::endl;

	tss.join();
	tdd.join();
}

NOT : Threadler simultaneously olarak çalıştı.


----------------------------------------------------------------------------------------------

ÖR:

std::promise<int> foo(int x, int y) // func promise döndürecek. Buda olabilir
{
	std::promise<int> prom;

	prom.set_value(x * y);

	return prom;
}


----------------------------------------------------------------------------------------------

EXCEPTION NASIL ILETILIYOR

struct Div{
	void operator()(std::promise<int>&& prom, int x, int y)const
	{
		try{
			if(b == 0)
			{
				auto err_str = "divide by zero error " + std::to_string(a) + " / " + std::to_string(b);
				throw std::runtime_error(err_str);
			}
			prom.set_value(a / b); // bölen 0 değilse işlemi yapıp, değeri set ediyor.
		}
		catch(...)
		{
			prom.set_exception(std::current_exception()); // Eğer bölen yukarıda 0 ise buraya girer
														  // promise in set_exception funcı ile current_exceptionun return değeri verildi
		}												  // Böylece shared state bir exception tutmuş oluyor. Value tutmuyor.
	}
};

void func(int x, int y)
{
	std::promise<int> prom;
	std::future<int> ftr = prom.get_future();
	std::thread th(Div{},std::move(prom),x,y);

	try{
		std::cout << x << " / " << y << " = " << ftr.get() << '\n';
	}
	catch(const std::exception& ex)
	{
		std::cout << "exception caught: " << ex.what() << '\n';
	}

	th.join();
}


int main()
{
	func(12,7);

	func(5,0); // Burada exception gönderilecek ve yakalanacak
}



NOT : Bu bir kereliğine kullanılacak iletişim kanalı. Biz 2 threadin 
sürekli iletişim halinde olmasını sitersek başka araçlar kullanacağız.

----------------------------------------------------------------------------------------------

BİR PROMISE NESNESİNİ 2 KEZ SET EDERSEK STD::FUTURE_ERROR TÜRÜNDEN HATA GÖNDERİR
AYNI ŞEKİLDE BİR FUTURE NESNESİNİ 2 KEZ GET EDERSEKTE FUTURE ERROR 

#include <future>
#include <iostream>

int main()
{
	std::promise<int> prom;
	prom.set_value(10);

	try{
		prom.set_value(20); // burada exception throw eder. 2. kez set edilmeye çalışılmış.
	}
	catch(const std::future_error& ex)
	{
		std::cout << "exception caught : " << ex.what() << '\n';
	}
}

----------------------------------------------------------------------------------------------

#include <future>
#include <iostream>

int main()
{
	std::promise<int> prom;
	auto ft = dprom.get_future();
	prom.set_value(10);

	auto val = ft.get();
	std::cout << "val = " << val << '\n';

	try{
		auto x = ft.get(); // yine exception throw eder. 2. kez get edilmiş.
	}
	catch(const std::future_error& ex)
	{
		std::cout << "exception caught : " << ex.what() << '\n';
	}
}

----------------------------------------------------------------------------------------------

VALID FUNC ÖRNEĞİ

int foo()
{
	// code
	return 1;
}

int main()
{
	auto ftr = std::async(foo);

	if(ftr.valid())
	{
		std::cout << "future is valid, get function can be called\n!"; // bu kısma girer
	}
	else
	{
		std::cout << "future is invalid, get function cannot be called\n!";
	}
	
	auto ival = ftr.get();
	std::cout << "ival = " << ival << '\n';

	if(ftr.valid())
	{
		std::cout << "future is valid, get function can be called\n!";
	}
	else
	{
		std::cout << "future is invalid, get function cannot be called\n!"; // artık değer alındı. yani get çağrıldı. valid değil.
	}																		// bu bloğa girer	

	try{
		auto i = ftr.get(); // get çağırmaya çalışırsak exception throw eder.
	}
	catch(const std::future_error& ex)
	{
		std::cout << "exception caught : " << ex.what() << '\n';
	}

}

----------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------

EĞER BİRDEN FAZLA KEZ GET FUNCI ÇAĞIRMAK İSTERSEK O ZAMAN SHARED_FUTURE SINIFINI KULLANACAĞIZ

shared future ın get funcını birden fazla kez çağırabiliriz. Tek fark bu.
Yani biz shared_future nesnesini ayrı ayrı threadlere gönderip orada herbirinin get funcını çağırabilirim.
Bu future olsaydı 2. kez get funcını çağrıınca exception throw edecekti.

Shared future sürekli bir iletişim olarak kullanılabilir demek değil. Yani promise nesnesini
2. kez set edebiliriz demek değil. Bir kere set edilip istenildiği kadar get çağrılabilir.


ÖR:

#include <future>
#include <iostream>
#include <thread>
#include <utility>
#include <syncstream>

struct SumSquare {
	void operator()(std::promise<int>&& prom, int a, int b)const
	{
		prom.set_value(a * a + b * b);
	}
};

void func(std::shared_future<int> sftr)
{
	std::osyncstream os{std::cout};
	os << "threadId(" << std::this_thread::get_id() << "): ";
	os << "result = " << sftr.get() << std::endl;  // hangi thread çalıştırırsa çalıştırsın, 
													// aynı promise in set ettiği değeri alacağız.
}

int main()
{
	std:promise<int> prom;

	std:shared_future<int> sftr = prom.get_future(); // DİKKAT!!! shared_future<int> olarak tutuyoruz.
													 // Shared future için auto imkanı yok burada

	std::thread th(SumSquare{}, std::move(prom),5,9); // aldığı promise i set etti.
	// NOT : Shared future kopyalanabilir !!!!!! future kopyalanamaz !!!!!!!!!
	
	// func functionda kopyalama semantiği var.Shared future copy semantics i destekliyordu.
	std::thread t1(func, sftr);
	std::thread t2(func, sftr);
	std::thread t3(func, sftr);
	std::thread t4(func, sftr);
	std::thread t5(func, sftr);

	th.join(); 

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
}


NOT : 
endl manipulatoru yerine
std::cout << "C++ dersi << "\n" << std::flush; kullanabiliriz

MOLA

==========================================================================================================================
==========================================================================================================================
==========================================================================================================================
==========================================================================================================================
==========================================================================================================================

Shared future elde etmenin 2 yolu var.
1 - Promise in get_future funcını çağırmak ama return değerini shared_future türünden nesnede tutmak
2 - Future sınıfının share isimli bir funcı var. Elimizde bir future nesnesi varsa ama bunu paylaşımlı
	olarak kullanmak istiyorsak future dan shared future elde etmek gerekiyor, bunun içinde sınıfın
	share funcını çağırmak gerekiyor.

Share funcını çağırdıktan sonra


ÖR: Bir öncekiyle benzer 

#include <future>
#include <iostream>
#include <thread>
#include <utility>
#include <syncstream>

struct SumSquare {
	void operator()(std::promise<int>&& prom, int a, int b)const
	{
		prom.set_value(a * a + b * b);
	}
};

void func(std::shared_future<int> sftr)
{
	std::osyncstream os{ std::cout };
	os << "threadId(" << std::this_thread::get_id() << "): ";
	os << "result = " << sftr.get() << std::endl;
}

int main()
{
	std::promise<int> prom;

	std::future<int> ftr = prom.get_future();

	std::cout << "ftr is " << (ftr.valid() ? "valid" : "invalid") << '\n'; //valid

	std::thread th(SumSquare{}, std::move(prom), 5, 9);

	std::cout << "ftr is " << (ftr.valid() ? "valid" : "invalid") << '\n'; //valid

	std::shared_future<int> s_ftr = ftr.share();
	// auto sftr = ftr.share(); artık burası auto ile de yazılabilir çünkü ftr bir future nesnesi

	std::cout << "ftr is " << (ftr.valid() ? "valid" : "invalid") << '\n'; // invalid
	(void)getchar();

	std::thread t1(func, s_ftr); // Artık bu shared future nesnesini başka threadlere kopyalama yoluyla göndeebiliriz.
	std::thread t2(func, s_ftr);
	std::thread t3(func, s_ftr);
	std::thread t4(func, s_ftr);
	std::thread t5(func, s_ftr);

	th.join();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
}

Output
------
ftr is valid
ftr is valid
ftr is invalid

threadId(11976) : result = 106
threadId(15836) : result = 106
threadId(3464) : result = 106
threadId(17820) : result = 106
threadId(3384) : result = 106

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

WAIT_FOR
--------
Future nesnesini belirli bir süre bekletiyor. Bir süre veriyoruz ve o süre kadar beklemesini istiyoruz.
wait_for un return değeri bir enumeration türü.

return değeri aşağıdakiler olabilir

future_status::deferred  =  Lazy evaluation yapılmıştır. Get funcını çağırınca func çalıştırılacak
future_status::ready  =  İş bitmiş değer hesaplanmış, promise nesnesi set edilmiş ve değer kullanılabilir
future_status::timeout  =  Bizim verdiğimiz değer geçilmiş ama halen değer hazır durumda değil.

Bekletmek demek, onu oluşturan thread arada başka işler yapabilir demek. 
Birde beklemek istemezsek ama return değerine bakmak istiyoruz. Hazır olup olmadığına bakmak istiyoruz.
Buna duration olarak 0 değerini verirsek yapabiliriz.Doğrudan return değeri ready yada deferred olacak.


BU ÖRNEK ÇOK ÖNEMLİ !!!!!!!!!!!!!

ÖR:

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std::literals;

void func(std::promise<int> iprom) // parametre sağ taraf referansı değil ama çağrı yaparken sağ taraf değeri göndermeliyiz
{									// çünkü bunun copy ctoru yok move ctoru var.
	std::this_thread::sleep_for(5s);

	iprom.set_value(20);
}

//200ms yazan yere bunuda yazabiliriz tabi.Chrono kütüphanesi hem zevkli hemde çok kullanışlı.
using halfsec = std::chrono::duration<double,std::ratio<1,2>>;

int main()
{
	std::promise<int> prom;
	auto ft = prom.get_future();
	std::thread th(func, std::move(prom)); // asenkron olarak bu thread çalışıyor.

	std::future_status status{}; // wait for return değeri türünden değişken

	//DİKKAT!!!!!!
	do {
	
	// status = ft.wait_for(halfsec{12.5});		Böylede olabilirdi.

	status = ft.wait_for(200ms); // Func içerisinden 5saniye yani 5000ms bloke edecek.
									// Döngünün her turunda wait_for funcının return değerini statusa atadık ve 200ms bekletiyoruz.
									// (Aslında döngüyü de bekletiyor 200ms, aynı zamanda değeri alıp kontrol ediyor hikayesi bu)
									// Bu beklerken ana thread ise çalışmaya devam ediyor.Func çalışması 5000mslerde sürdüğü için,
									// wait_for funcı çağırdık ve return değeri statusa aktarıldı.wait_for burada timeout dönecek.
									// wait_for un return değeri ready olduğunda döngüden çıkacak. Bu sırada bu döngü içinde diğer işlerde
									// yapılmaya devam edecek. Burada future ın get funcı çağrılsaydı, burada funcın çalışması
									// devam edecektik biz bunu bekleyecektik ve başka bir iş yapmayacaktık.
									// Burada ise her 200ms de bir işin tamamlanıp tamamlanmadığına bakıyoruz.
									// tamamlanmamışsa diğer işler yapılmaya devam ediyor.
									// wait_for return değeri future_status::ready olduğunda döngüden çıkacak ve get funcı çağırınca değer
									// hazır olacak.


	std::cout << "... doing some work here\n" << std::flush;

	} while (status != std::future_status::ready);

	std::cout << "the return value is " << ft.get() << '\n';

	th.join();
}

--------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

constexpr int x = 50;

long long fib(int n)
{
	return n < 3 ? 1 :  fib(n - 1) + fib(n - 2);
}

int main()
{
	using namespace std::literals;

	auto ftr = std::async(fib, x); //async kullandık.

	std::cout << "bekle cevap gelecek\n";

	while (ftr.wait_for(10ms) == std::future_status::timeout)
	{
		std::cout << '.' << std::flush;
	}

	auto result = ftr.get();

	std::cout << "fib(" << x << ") is : " << result << '\n';
}


Output
------
bekle cevap gelecek
.............................................
.............................................
.............................................
............................................. fib(50) is : 12586269025

--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------

PACKAGE TASK
------------
Kopyalamaya kapalı ama taşımaya açık bir sınıf.Package task sınıfı türünden nesne oluşturunca bir thread yaratılmıyor !!!
Bir callable ı sarmalıyor.std::function gibi ama bu sarmalanan callable ı asenkron olarak çalıştırmak için kullanılıyor.
Package taskin get feature funcı var. Bu func package taskın arka planda kullandığı promise in
future ını verecek. Aradaki fark, biz funcın çağrılması için package taski func call operator funcını çağırıyoruz.
Async olsaydı, async çağrılır çağrılmaz thread çalışmaya başlıyor. Package task callable ı sarmalıyor
istediğin zaman çalıştırabilirsin diyor.Taski istediğin yere gönderip ne zaman çağırmak istersem func call
operator funcı çağrılır.

Package taskin template parametresi bizden callable ın türünü istiyor. Aynı std::functionda olduğu gibi.

Async ile package task arsındaki fark, async e verdiğimiz argümana göre callable ı asenkron olarak çalıştırıp
bize future nesnesini veriyor ama package task nesnesi oluturunca bir function çağrılmıyor. Func çağrılması için
func call operator funcının çağrılması gerekiyor. Yine future promise yapısı var. yine package tasking get future funcı ile
future nesnesini elde ediyoruz.

Header file future

ÖR:

#include <future>
#include <iostream>

int foo(int x, int y)
{
	return x * y + 5;
}

int main()
{
	std::packaged_task<int(int,int)> ptask{foo}; //CTAD çalışır std::packaged_task ptask{foo}

	ptask(12,45); // Burada function çağrılır. std::function gibi sarmalıyor.Fark burada asenkron çağrı yapmak.
		
	//-------------------------------------------------------------------

	std::packaged_task ptask{foo};
	
	std::future<int> ftr = ptask.get_future(); // auto kullanılabilir.

	//Funcın fiilen çağrılması için ptaskin func call operator funcını kullanmamız gerekiyor.

	ptask(12,45);

}

--------------------------------------------------------------------------------------------------------------

ÖR:

int main()
{
	// aşağıda ctad da kullanabilirdik.
	std::packaged_task<double(double, double)> ptask([](double a, double b) {
			return std::pow(a, b) + std::pow(b, a);
		});

	//std::future<double> result = ptask.get_future();  buda geçerli
	auto result = ptask.get_future(); // future nesnesini aldık

	ptask(1.2, 3.4); // func çağrıldı

	std::cout << "result : " << result.get() << '\n';  // get funcı çarıldığında func return değerini alacağız.
}

--------------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <utility>
#include <future>
#include <thread>

int main()
{
	using ftype = int(int, int); 
	
	std::packaged_task<ftype> pt_x;
	
	std::packaged_task<ftype> pt_y([](int x, int y)
									{return x * x + y * y; });
	

	// pt_x = pt_y; // Bu sentaks hatası olurdu. Kopyalanamaz ama taşınabilir.

	pt_x = std::move(pt_y); // pt_y boşa çıktı. pt_x workloadu aldı.

	std::future<int> ret = pt_x.get_future(); // future nesnesi pt_x ten alındı
	
	std::thread(std::move(pt_x), 5, 15).detach(); // threade ptx argüman olarak gönderildi 
												  // package task bir callable, threadde callable alıyor zaten
												  // std::move burada da kullanmak zorundayız.
												  
	auto result = ret.get(); // çağrılan callable ın return değerini aldık.
	
	std::cout << "the result is " << result << "\n";
}


*/
