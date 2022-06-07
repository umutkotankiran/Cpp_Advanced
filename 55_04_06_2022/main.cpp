/*

Gruptan sorduğu soruyu cevapladı hoca

Bugün yeni bir araç tanıtılacak.

COROUTINES
----------
İlk önce github notlarını koyuyorum.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

GITHUB NOTLARI
--------------
- Coroutin'ler 1958 yılında Mel Konvoy tarafından icad edildi.

- Coroutine kendi çalışmasını durdurabilen (suspend) daha sonra çalışması tekrar devam ettirilebilen bir fonksiyon. Bu nedenle coroutine'lere "resumable function" da deniyor. 
  Bu açıdan bakarsak fonksiyonları 2 kategoriye ayırabiliriz:
	-- Subroutines (normal fonksiyonlar)
	-- Coroutines (çalışması durdurulup sonra tekrar devam ettirilebilen fonksiyonlar)

- C++20 standartları ile dile bu konuda eklemeler yapıldı. Hem dile bazı temel araçlar (core features) hem de standart kütüphaneye bazı öğeler eklendi. Ancak yine de bu araçtan faydalanmak için en basit kullanım senaryolarında dahi bazı bileşenlerin programcılar tarafından yazılması gerekiyor. C++23 ile standart kütüphaneye destekleyici bazı öğelerin eklenmesi planlanıyor.

- Normal fonksiyonlar, çağrıldıkları zaman kodlarının tamamı çalışıyor. Yani fonksiyonun çalışması ya bir return deyimi ile ya da bir exception gönderilmesi ile sonlanıyor. Oysa bir coroutine birden fazla adıma bölünerek çalıştırılabiliyor. Yani fonksiyonun çalışması durdurulup (suspend) tekrar başlatılabiliyor. Bu işlem akışı birden fazla kez gerçekleştirilebiliyor.

- Neden bir fonksiyonu bu şekilde çalıştırmak isteyelim?
	-- Fonksiyon belirli bazı işlemleri gerçekleştirdikten sonra işine devam etmek için bazı başka işlemlerin yapılmasını bekleyebilir.
	-- Fonksiyon belirli bir basamakta elde ettiği ara veri ya da verileri kendisini çağıran fonksiyona iletebilir.
	-- Bir coroutine çağırdığımızda onun kodunu basamaklar (steps) halinde çalıştırabiliyoruz. Bu paralel çalıştırma (parallelism) ile karıştırılmamlı. (ping pong oyunu gibi düşünebiliriz.)

- Hem ana kontrol akışı hem de coroutine'in kendi kontrol akışı aynı thread içinde gerçekleşiyor. multi-thread programlama ya da eş zamanlı erişim oluşturmak zorunda değiliz. Ancak coroutine'leri farklı thread'lerde çalıştırmak da mümkün.

- Genel olarak programlama dillerindeki coroutine'ler iki ana kategoriye ayrılıyor:
	-- Stackless coroutine'ler
		--- stackful coroutine'ler C++ dili stackless coroutin'ler sunuluyor.

-- C++ dilinde, bir fonksiyonun coroutine olup olmadığı bildiriminden değil tanımından (implementation) anlaşılıyor. Yani bir fonksiyonun sadece bildirimine bakarak onun coroutine olup olmadığını anlayamıyoruz. Eğer fonksiyon tanımı içinde aşağıdaki anahtar sözcüklerden biri var ise derleyici söz konusu fonksiyonu bir coroutine olarak ele alıyor:
	-- co_await
	-- co_yield
	-- co_return

Ancak bir fonksiyonun coroutine olabilmesi için geri dönüş türünün bazı şartları sağlaması gerekiyor.

- Coroutine'ler için C++20 itibarıyla aşağıdaki kısıtlamalar söz konusu:
	-- Bir coroutine içinde return statement kullanılamaz. 
	   Yalnızca co_return ya da co_yield statement kullanılabilir. ancak co_return statement kullanılması zorunlu değil.
	-- Coroutine C tarzı bir variadic fonksiyon olamaz.
	-- Coroutine constexpr fonksiyon olamaz
	-- Bir constructor ya da destructor coroutine olamaz.
	-- main fonksiyonu coroutine olamaz.
	-- coroutine bildiriminde auto return type kullanılmaz.

Derleyici bir coroutine için nasıl bir kod üretiyor?.
- Bu konu bir hayli karmaşık. Öncelikle derleyicinin, programcı tarafından tanımlanacak bazı sınıflara ve fonksiyonlara 
  güvenerek kod ürettiğini söyleyerek başlayayım. Standart kütüphane şimdilik doğrudan kullanılacak bazı sınıflar sunmuyor. 
  C++23 standartları ile standart kütüphaneye yeni sınıfların ve fonksiyonların ekleneceği belirtiliyor. 
  Konunun daha iyi anlaşılmasına fayda sağlayacağını düşündüğümden coroutine'lerin gerçekleştiriminde kullanılan bileşenlerin 
  her birini daha sonra ayrı ayrı ele alacağım.

- Derleyicinin coroutine için bir "coroutine frame" oluşturması gerekiyor. Bunun için bir bellek alanına ihtiyacı var. coroutine frame'de hangi bilgiler tutuluyor?
	-- coroutine parametre değişkenleri
	-- tüm yerel değişkenler
	-- bazı geçici nesneler
	-- coroutine suspend edildiğindeki excecution state (register'lar instruction pointer vs.)
	-- çağıran koda iletilecek değer ya da değerleri tutacak olan bir promise nesnesi.

- Genel olarak coroutine frame dinamik olarak edinilmek zorunda. coroutine suspend edildiğinde (durdurulduğunda) 
  stack erişimini kaybediyor. coroutine frame'in oluşturulması için operator new kullanılıyor. Ancak farklı ihtiyaçlar için 
  operator new yüklenebiliyor (overload edilebiliyor).

- coroutine frame coroutine'in çalıştırılmaya başlanmasından önce oluşturuluyor. 
  (normal fonksiyonlarda stack frame'in oluşturulması gibi). Derleyici coroutine frame'i, çağıran koda, 
  coroutine frame'e erişimi sağlayacak bir handle döndürüyor (ama doğrudan değil)

COROUTINE HANDLE
Peki coroutine frame'e nasıl erişeceğiz? Standart kütüphane bu amaçla bize std::corutine_handle sınıf şablonunu sunuyor. 
Bu sınıfı iyi anlamalıyız:

template<typename T = void>
struct coroutine_handle;

template<>
struct coroutine_handle<void> {
	coroutine_handle()noexcept = default;
	coroutine_handle(std::nullptr_t)noexcept;
	coroutine_handle& operator=(std::nullptr_t)noexcept;
	explicit operator bool()const noexcept;
	static coroutine_handle from_address(void* adr)noexcept;
	void* to_address()const noexcept;
	void resume()const;
	void destroy();
	bool done()const;
};

template <typename Promise>
struct coroutine_handle : coroutine_handle<Promise> {
	Promise& promise()const noexcept;
	static coroutine_handle from_promise(Promise&)noexcept;
};


- Yukarıdaki koddan görülebileceği gibi std::coroutine sınıf şablonunun void açılımı (specialization) için bir explicit specalization verilmiş. 
  Diğer tür argümanları ile oluşturulacak sınıflar coroutine_handle sınıfından kalıtım yoluyla elde ediliyorlar.
 
- Promise nesnesi (promise object)

- Promise nesnesi coroutine frame içinde tutuluyor.

- Promise nesnesi, coroutine fonksiyonun içinden manipüle ediliyor.

- Coroutine ürettiği değer ya da değerleri kendisini çağıran koda promise nesnesini kullanarak iletiyor.

- co_await için nasıl bir kod üretiliyor?
  Bir co_await ifadedsinin aşağıdaki gibi kullanıldığını düşünelim:

  auto result = co_await expr;


Awaitables
- co_await operatörünün ihtiyaç duyduğu operandlara Awaitables deniyor. Yani co_await operatörünü kullanabilmemiz için bir Awaitable gerekiyor.
- Bir Awaitable elde etmenin tipik biçimine Awaiter deniyor. cort'nin suspen ya da resume olması için awaiter'ın 3 üye fonksiyona sahip olması gerekiyor.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

COROUTINES
----------
Dersten devam ediyorum.
Çok önemli bir araç.C++20 ile eklendi.Bu bir function ama farkı var.
Bir kısmını C++23 e bırakmışlar o yüzden şuanki hali programcı açısından bunu kullanmak ciddi bir yük.
Programcıyı zorluyor. Fazla kod yazmak gerekiyor çünkü hazır bazı sınıfları eklememişler henüz.
Concurrency ye ciddi bir alternatif oluşturuyor ve konu baya karışık deniyor.
Tasarımdaki temel felsefe çok fazla sınır koymamak ve programcının esnek olarak kullanmasını sağladığı için çok zor.

Kullanılan başka bir terim resumable function.

Bizim alıştığımız function modelinin dışında öyle bir model ki function çalıştırıldıktan 
sonra durdurulabiliyor, çalışması suspend edilebiliyor. Yani function çalıştırıldı ve func birşeyler
yaptı ve kendini suspend etti, tekrar kontrol funcı çağıran koda geçti, kontrol eden kod isterse
tekrar functionu o suspend ettiği yerden tekrar çalışmasını sağlayabilir.

Yani func kodu çalışırken çalışması duruyor, kontrol çağıran koda geçiyor ve tekrar devam
et derse devam ettirebiliyor. Bu birden fazla kerede yapılabiliyor.

Bu özellikle concurrency konusunda os scheduleing vs. mekanızlamarının doğrudan devreye
girmediği yerlerde bu işlemlerin yapılabilmesini sağlıyor.

Neden Coroutine deniyor?
C++ ta functiona normalde subroutine deniyor, buna ise coroutine.

Nasıl oluyorda bir funcın çalışması durduruluyor ve biz onu o noktadan devam ettirebiliyoruz?
Bunun yapılması için bir şekilde funcın durduğu yerdeki bilgilerin erişilebilir hale gelmesi gerekiyor.
Yani bunun bir nesne şeklinde temsil edilmesi gerekiyor. Buda şu demek funcın tekrar çalışması için
gereken bilgilerin biryere yazılması gerekiyor. Bunlar funca gönderilen argümanlar, yerel değişkenler
registerların durumları. Bu açıdan bakınca coroutine ler 2 ye ayrılıyor.

Stackless : Bu bilgileri bir yerde tutuyorum ama tüm stackin bilgilerini tutmuyorum. 
			Sadece kullanacağım değişkenler ve state bilgisini biryerde tutuyorum.

Stackfull : Burada ise stackin tamamını tutuyor.

Subroutine ve coroutine farkını gösteren resmi githuba koydum
subroutine&coroutine.jpg

Burada çalışan func kendini suspend etti bir değer gönderdi, karşı taraf tekrar devam ettirdi
ve tekrar bu func suspend ettiğinde yine bir değer gönderir.Bu şekilde devam edebilir.

Funcın çalışması suspend etti diyelim, bir threadden devam ettiriliyordu ama farklı bir thread
den de devam ettirilebilir.yani resumption farklı threadden olabilir.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

State bilgisinin bir yerde tutulması gerekiyor. Genelde compiler optimizasyon yapmazsa bu bilgi 
heapte tutuluyor. 

Burada işi karmaşık hale getiren bu mekanizmanın sınırlandıcı şekilde değilde daha custom
hale getirilmesi. C++23 te muhtemelen bu hazır sınıflar gelecek daha kolay kullanım için.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Bir funcın subroutine mi coroutine mi olduğu funcın bildiriminden kolayca anlaşılmıyor.

Myclass func(int,int); // Bu coroutine mi subroutine mi? doğrudan anlaşılmıyor

Coroutine olduğunu derleyicinin implementasyonundan anlıyor.
Burada kullanılan bazı keywordlerden anlıyor.
- co_await
- co_yield
- co_return

Yukarıdaki keywordler bir functa varsa coroutine olarak değerlendiriliyor bu func.
Derleyicinin bir kodu coroutine olarak değerlendirmesi farklı bir kod üretmesi anlamına geliyor.
Eğer coroutine olarak değerlendirirse compiler, bir coroutine frame oluşturması gerekiyor ve bir
allocation yapılması gerekiyor. Bunun dışında functionun return değeri türünün belirli özellikleri
sağlaması gerekiyor.

Burada öğrenmeyi zorlaştıran nokta, coroutine return türünü oluşturmamız gerekiyor ve
bu türe bağlı promise type eş ismine sahip bir sınıfı oluşturmamız gerekiyor. 
Çünkü derleyicinin implementasyonu bazı sınıfların ve funcların var olmasına göre oluşturulmuş.
Yani ilgili sınıfı veya funcı implemente etmezsek bu durumda sentaks hatası olur.

Coroutine in return değeri bir nevi coroutine interfacei.Return değerini biz yazıyorduk yukarıda yazdım.

Coroutinelerin retrurn değeri doğrudan datanın kendisi değil ama coroutine i yönetmek ve yönlendirmek için,
kullanacağımız aslında coroutini temsil eden, çalışması devam etmekte olan funcın adeta yerine geçen bir nesne gibi
düşünülebilir.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Coroutine State

Bu state coroutine frame de tutuluyor.
Bunda promise nesnesi, parametre değişkenlerinin kopyaları,yerel değişkenler
ve func resume edildiğinde ona yönelik bilgiler tutuluyor.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Functionlar

await ready : Coroutine suspend edilmeden hemen önce çağrılıyor.True dönerse coroutine suspend edilmiyor
			  suspend için false dönmeli.

await suspend : Coroutine suspend edildikten sonra awaiter sınıfının await suspend funcı çağrılıyor.
				Buarada özel bir takım şeyler yapılabilir.

await resume : Coroutine resume edildikten sonra çağrılıyor.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Keywordler

co_await :  Bu sadece suspend edilmesini istediğimizde kullanıyoruz.Suspension mekanizması dışarıdan çalıştırılmıyor.
			coroutine ne zaman suspend olacağını kendisi belirliyor. Coroutine içeriden suspend ediliyor
			dışarıdan resume ediliyor. Bu keyword zaten coroutine içerisinden yazılmış

			Bu suspend ederken, resume edecek koda yani coroutine i kullanan koda bir değer
			göndermiyor.


co_yield :	Bu return ün özel birimi gibi görülebilir ama teknik olarak öyle değil aslında.
			co_await ile neredeyse aynı farkı ise co_await sadece suspendi sağlıyor, 
			co_await suspend etmekle birlikte çağıran koda bir değer iletiyor.
			yani suspend oldum 5 değerini gönderdim, tekrar suspend oldum 15 değerini gönderdim gibi.
			  

co_return : coroutine in çalışmasının sonlanıp tamamen return etmesi için.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Coroutine Kısıtlamaları - Neler coroutine olamaz

- Sınıfların constructorları coroutine olamaz
- C tarzı variadic funclar coroutine olamaz
- Return deyimi kullanamıyoruz.Yukarıdaki keywodlerden biri olacak
- auto return type kullanamıyoruz.(trailing değil)

Sınıfın member funcı veya global bir func coroutine olabilir.
Lambda functionlar coroutine olabilir.

Summary diye bir resim koydum orada özetler var.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Standartlar bize 2 tane sınıf veriyor. 
suspend_always ve suspend_never

Eğer suspend edilmesini istemezsek ve başka bir customizasyona ihtiyaç yoksa
co_await ifadesine operand olarak suspend_never geçeceğiz
(co_await.jpg ye bakarsan burada await_ready nin değili işleme sokulmuş. bu şekilde düşününce oturuyor olay)

Suspend edilmesini istersek suspend_always 

class suspend_always
{
	constexpr bool await_ready() const noexcept { return false; }  // sadece fark burada. diğeri return true
	constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
	constexpr void await_resume() const noexcept {}
};


class suspend_never
{
	constexpr bool await_ready() const noexcept { return true; }
	constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
	constexpr void await_resume() const noexcept {}
};


CO_AWAIT.JPG YI BURAYA DA KOYUYORUM !!!

										  auto && __a = expression;
										  if(!__a.await_Ready())
										  {
auto result = co_await expression; --->			__a.await_suspend(coroutine-handle);
												//suspend/resume point
										  }

										  auto result = __a.await_resume();


Bazı durumlarda co_await i biz kullnırken bazı durumlarda da derleyici bir ifadeyi co_await operatörünün
kullanımına dönüştürüyor.

Bizim minimal başlangıç noktamız bir coroutine func yazıp bunun derlenebilmesi için bunun return değerinin
belirli garantileri sağlayan bir tür olması gerekiyor. Buradan sonra aşağıdaki örneğe geçiyorum

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

COROUTINE_HANDLE TEMPLATE

template<typename T = void>
struct coroutine_handle;

template<>
struct coroutine_handle<void>
{
	coroutine_handle()noexcept = default; // default ctor
	coroutine_handle(std::nullptr_t)noexcept; // nullptr ile coroutine handle init edilebilir
	coroutine_handle& operator=(std::nullptr_t)noexcept;  // nullptr atanabilir
	explicit operator bool()const noexcept;   // operator bool funcı
	static coroutine_handle from_address(void* adr)noexcept;   // bir adresten coroutine handle elde edebilir
	void* to_address()const noexcept;	// void * a döndüren func
	void resume()const; // ÖNEMLİ !!! İsimlendirme tuhaf. Bu resume ile awaiter sınıfının resume karıştırılmamalı
						// awaiterın resume funcı, resume edildikten sonra customizasyon amaçlı başka şeylerde yapan func
	void destroy();		// coroutine handle resume ise doğrudan resume işlemini yapıyor.
	void done()const;  // buda destroy ediyor. 
};

Diğer açılımlarda coroutine handle ın void açılımından elde edilmiş.


--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MİNİMAL BİR COROUTINE ÖRNEĞİ.

1 - Coroutine yazıldı
2 - Funcın return değeri olan tür yazıldı.Corapi
3 - Maini yazdık
4 - class içerisine promise type ve handle(CrtHandle) yazıldı.
5 - CrtHandle veri elemanı yazıldı
6 - Special member funclar ve resume yazıldı.
7 - Promise type ın member funcları var onlar yazıldı

Aslında bir sonraki örneği ilk yaptık ama yer değiştirdim bu daha basit.

#include <iostream>
#include <coroutine>
#include <exception>

class Corapi
{
public:
	struct promise_type;
	using CrtHandle = std::courotine_handle<promise_type>;
	Corapi() = default;
	Corapi(const Corapi&) = delete;
	Corapi(const CrtHandle&) : m_h{h} { };
	Corapi& operator=(const Corapi&) = delete;
	bool resume()const
	{
		if(!m_h || m_h.done())
		{
			return false;
		}

		m_h.resume();
		return !m_h.done();
	}
private:
	CrtHandle m_h = nullptr;
};



//Promisetype.jpg de member funcları var
// Burada zorunlu olanlar var.Promise_type.jpg de var.
struct Corapi::promise_type		  //sınıf içidne de yazabilirdik.
{
	auto get_return_object()
	{
		return Corapi{crtHandle::from_promise(*this)};
	}
	
	auto initial_suspend()
	{
		return std::suspend_always{};
	}
	
	auto final_suspend()noexcept
	{
		return std::suspend_always{};
	}

	void unhandled_exception()
	{
		return std::terminate();
	}

	void return_void()
	{
		//bos
	}

};



// burası coroutine
Corapi cprint(std::string name)
{
	// Burada suspend edecek mi bunu belirleyen benim promise typeım
	
	std::cout << name << " hosgeldiniz \n";
	co_await std::suspend_always{};

	std::cout << "Nasilsiniz ? << name << " Bey\n";
	co_await std::suspend_always{};

	std::cout << "tanistigimiza memnun oldum ?\n";
	co_await std::suspend_always{};

	// Burada suspend edecek mi bunu belirleyen benim promise typeım
}

int main()
{
	auto coreapi = cprint("Harun");

	int cnt;
	while(coreapi.resume())  // promise type yazılmazsa resume çalışmıyor zaten.Bunu Corapi içine yazıyoruz
	{
		std::cout << "count = " << ++cnt << '\n';
	}

}


--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

BİZ AWAITER OLARAK SUSPEND ALWAYS KULLANDIK BUNU DEĞİŞTİRECEĞİZ.

#include <iostream>
#include <coroutine>
#include <exception>

class Corapi
{
public:
	struct promise_type;
	using CrtHandle = std::coroutine_handle<promise_type>;
	Corapi() = default;
	Corapi(const Corapi&) = delete;
	Corapi(const CrtHandle&h) : m_h{ h } { };
	Corapi& operator=(const Corapi&) = delete;
	bool resume()const
	{
		if (!m_h || m_h.done())
		{
			return false;
		}

		m_h.resume();
		return !m_h.done();
	}
private:
	CrtHandle m_h = nullptr;
};


struct MyAwaiter {
	bool await_ready()noexcept
	{
		std::cout << "Myawaiter::await_ready cagrildi ve simdi suspend edilecek\n";
		return false;
	}

	void await_suspend(std::coroutine_handle<>)noexcept // coroutine_handle<> boş bırakırsam void açılımı demek.
	{
		std::cout << "suspend edildikten sonra await_suspend cagrildi\n";
	}

	bool await_resume()noexcept
	{
		std::cout << "resume edildikten sonra await_resume cagrildi\n";
		return false;
	}

};


//Promisetype.jpg de member funcları var
// Burada zorunlu olanlar var.Promise_type.jpg de var.
struct Corapi::promise_type		  //sınıf içidne de yazabilirdik.
{
	auto get_return_object()noexcept
	{
		return Corapi{ CrtHandle::from_promise(*this) };
	}

	auto initial_suspend()
	{
		return MyAwaiter{}; //Dİkkat !!!
	}

	auto final_suspend()noexcept
	{
		return MyAwaiter{}; //Dİkkat !!!
	}

	void unhandled_exception()
	{
		return std::terminate();
	}

	void return_void()
	{
		//bos
	}

};


// Düzenledik burayı
Corapi cprint(std::string name)
{
	std::cout << name << " hosgeldiniz \n";
	co_await MyAwaiter{};

	std::cout << "tanistigimiza memnun oldum ?\n";
}

int main()
{
	auto coreapi = cprint("Harun");

	std::cout << "Resume oncesi\n";
	int cnt{};
	while (coreapi.resume())  // promise type yazılmazsa resume çalışmıyor zaten.Bunu Corapi içine yazıyoruz
	{
		std::cout << "count = " << ++cnt << '\n';
	}

}



--------------------------------------------------------------------------------------------------------------------------------------------------------------------------


ÇOK ÖNEMLİ ÖRNEK !!!!!!!!!!!!!!!!!!!!!!!
Açıklamaları yan taraflarına yazacağım

#include <iostream>
#include <coroutine>


// 2.
// Func kodunun derlenebilmesi için
// func return değerinin belirli garantileri sağlayan bir tür olması gerekiyor.
class Coret {
public:
	struct promise_type;  // 3. Olmazsa olmaz nested type olan bir promise type olmak zorunda

	using CoroHandle = std::coroutine_handle<promise_type>;  // 4. Olmazsa olmaz 2 nokta bir coroutine handle değeri
															// bu coroutine handle tamamen bizi coroutine frameine eriştirecek nesne
															// std library coroutine_handle isimli bir sınıf şablonu var
															// Bu sınıf şablonunun önce void specializationu yapılmış, dolayısıyla
															// void special. ın bir interface i var, void dışındaki specializationlar
															// voidden kalıtımla elde edilmiş. Yani coroutine_handle ın void açılımının interfacei
															// coroutine_handle ın başka bir açılımının interfaceinin bir parçası ama 2 funcı daha var.

private:
	CoroHandle mp_handle{ nullptr };  // 5. Aslında birçok şeyi yönlendiren bu olacak

public:
	explicit Coret(CoroHandle handle)noexcept : mp_handle{ handle } {}

	~Coret()
	{
		if (mp_handle)
			mp_handle.destroy();
	}

	Coret(const Coret&) = delete;	// kopyalamaya karşı kapatılmış ama taşımaya karşı açık olup olmaması bize bağlı.
	Coret& operator=(const Coret&) = delete;

	Coret(Coret&& other)noexcept : mp_handle{ other.mp_handle }
	{
		other.mp_handle = nullptr;
	}

	Coret& operator=(Coret&& other)noexcept
	{
		if (this != &other)
		{
			mp_handle = other.mp_handle;
			other.mp_handle = nullptr;
		}
		return *this;
	}

	bool resume()const	// coroutine handle resume funcının resume funcı ile karıştırılmamalı.
	{
		if (!mp_handle || mp_handle.done())
		{
			return false;
		}

		mp_handle.resume();
		return !mp_handle.done();
	}
};


// 6. Asıl yönetimi yapan sınıf bu.
struct Coret::promise_type {
	auto get_return_object() // Bu bir coret interface nesnesi döndürecek.
	{
		return Coret{ CoroHandle::from_promise(*this) }; // Derleyici promise nesnesinin konumunu biliyor ve promiseten hareketle
														// doğrudan handle ın gösterdiği nesnenin adresini elde ediyor.
	}

	std::suspend_always initial_suspend()		// Resumable_thing.jpg de bunlarada ihtiyaç olduğu görülüyor.
	{
		return {};		// bu funcın return değeri bir awaitable.Derleyici bu funcların return değerini co_awaitin operandı yapıyor.
	}

	std::suspend_always final_suspend()noexcept // bunada ihtiyaç var.
	{
		return {};
	}

	void unhandled_exception()	// jpg dosyasında yok ama orada aslında try bloğu var.o yüzden buda şart.
	{							// exception yakalanırsa unhandled exception functionu çağrılıyor.
								// yakalanamayan exception olursa bunu unhandled exceptiona koyacağız.
		std::terminate();
	}

	void return_void() { }  // function değer döndürmeyecekse bu funcı implemente edeceğiz çünkü derleyici bunu çağırıyor
							// function bir değer üretecekse return value funcını implemente ediyoruz.
};


// 1.
// Bizim coroutine bu
// Coret coroutine in return değeri türü. yani coroutine interface.
// Bu return değerinin belirli garantileri sağlayan bir tür olması gerekiyor.
// Bu nesne coroutine i temsil ediyor. Bu nesne vasıtasıyla çalışması yönlendirilebiliyor.
// bir takım değerler alabiliyoruz yada move semantics ile taşınmasınıda sağlayabiliriz.
// Coroutine olup olmadığı func definitiondan anlaşılıyor. co_await kullanılmış.
// Coroutine in amacı, ben her durdurulduğumda bir değer üreteyim beni resume edecek kodda bu değeri kullansın
// ise yani bir producer ve consumer ilişkisi olarak kullanılacaksa bunlara generator deniyor.
// return değeri generator ise bu bir coroutinedir.
// C++ 23 ile bu türlerin standart halleri gelecek.
// Burada Coreti bizim kendimiz yazmamız gerekiyor. Sadece coroutine return değeri yani coroutine
// interfacei değil bazı durumlarda awaitable denilen sınıfları implemente edip onlarında
// member funclarını yazmamız gerekiyor.

// 3 tane kontrol ve customizasyon noktası veriliyor.
// 1 - Doğrudan funcın return değeri olan tür. Coroutine in return ettiği tür
// 2 - Coroutine in yönlendirilmesini sağlayan onun davranışını belirleyen promise type
//	   Bunuda kendimiz oluşturup oluşturduğumuz type içn yapılacak şeylere bağlı olarak minimal
//     bazı functionları implemente ediyoruz.
// 3 - Awaiter sınıfları özelleştirmek için kendi awaiter sınıfımızı yazıyoruz.

// Aslında coroutine frame i gösteren bir handle var. Biz onu bir tür olarak kullanıyoruz.
// Bu handle tipik bir raii nesnesi değil. Daha çok normal bir ptr gibi. Derleyici ürettiği kodda
// coroutine frame i gösteren ptr oluşturuyor ama o ptr nin gösterdiği nesneyi free/delete etmek için
// yine burada oluşturulan sınıflardan birinin member funcını(destroy funcını) çağırmak gerekiyor.

Coret cofunc(int n)
{
	// Burada initial suspend olacak çünkü ona göre yazdık.

	for (int i = 0; i < n; ++i)
	{
		std::cout << "cofunc suspends i = " << i << '\n';
		co_await std::suspend_always{}; // döngünün her turunda program akışı buraya gelince
										// funcın çalışması durdurulacak
										// co_await bir operator. Yanındaki ifadenin ait olduğu türün
										// bazı funcları olmak zorunda çünkü derleyici co_await kullanımını
										// bir koda dönüştürüyor.co_await isimli resim yüklendi
										// co await burada suspend edilmesini sağlıyor.
										// bunu özelleştirebiliyoruz. Yani beklemesini,durmasını
										// tekrar kontrolün çağıran tarafa geçmesi durumda ilave birtakım
										// işlemlerin yapılmasını sağlayan co_awaitin operandı olan tür.
										// bu türlere awaitable türler demiş standartlar.
										// Derleyici await_ready, await_suspend ve await_resume kullanıyor.
										// Bunlardan biri olmasa sentaks hatası verir.
										// await_resume çağrıldığında kontrol geri dönmüyor
										// Bu isimlendirme hatası.
	}

	// Burada da suspend oluyor.
}


int main()
{
	auto cret = cofunc(5);
	int cnt{};

	while (cret.resume())
	{
		std::cout << "resume call : " << ++cnt << '\n';
	}
}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CO_AWAITE GERİ DÖNDÜK

co_awaitin operandının awaitable olması gerekiyor, aynı şekilde co_yield da öyle.
mesela 
co_await 234; // bu normalde sentaks hatası olur çünkü operandı awaitable değil
Bunu yapabilmenin yollarından biri sınıfın await_transform functionunun olması.

class CoroTask{
	struct promise_type{
		auto await_transform(int val)
		{
			return MyAwaiter{val};
		}
	};
};

CoroTask coro()
{
	co_await 42; // Artık Geçerli
}

Buradaki int parametre await_transformu çağıracak buda awaiter döndürecek.
Dolayısıyla co_awaitin operandının başka türden olması fakat await transform funcı ile o değeri alan bir
await transform funcitonu return değeri bir awaiter sınıfı türünden olabilir.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <coroutine>
#include <exception>

class Corapi
{
public:
	struct promise_type;
	using CrtHandle = std::coroutine_handle<promise_type>;
	Corapi() = default;
	Corapi(const Corapi&) = delete;
	Corapi(const CrtHandle&h) : m_h{ h } { };
	Corapi& operator=(const Corapi&) = delete;
	~Corapi()
	{
		if(m_h)
			m_h.destroy();
	}

	bool resume()const
	{
		if (!m_h || m_h.done())
		{
			return false;
		}

		m_h.resume();
		return !m_h.done();
	}
private:
	CrtHandle m_h = nullptr;
};


//Promisetype.jpg de member funcları var
// Burada zorunlu olanlar var.Promise_type.jpg de var.
struct Corapi::promise_type		  //sınıf içidne de yazabilirdik.
{
	auto get_return_object()noexcept
	{
		return Corapi{ CrtHandle::from_promise(*this) };
	}

	auto initial_suspend()
	{
		return std::suspend_always{}; //Dİkkat !!!
	}

	auto final_suspend()noexcept
	{
		return std::suspend_always{}; //Dİkkat !!!
	}

	void unhandled_exception()
	{
		return std::terminate();
	}

	void return_void()
	{
		//bos
	}

	auto await_transform(double val)
	{
		std::cout << "promise_tyoe::await_transform(double val) val = " << val << '\n';
		return std::suspend_always{};
	}

};


// Düzenledik burayı
Corapi cprint(std::string name)
{
	std::cout << name << " hosgeldiniz \n";
	co_await 5.6;										// DİKKAT!!!

	std::cout << "tanistigimiza memnun oldum ?\n";
}

int main()
{
	auto coreapi = cprint("Harun");

	std::cout << "Resume oncesi\n";
	int cnt{};
	while (coreapi.resume())  // promise type yazılmazsa resume çalışmıyor zaten.Bunu Corapi içine yazıyoruz
	{
		std::cout << "count = " << ++cnt << '\n';
	}

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DIĞER YOL OLARAK OPERATOR CO_AWAIT FUNCIDA YAZILABILIR


#include <iostream>
#include <coroutine>
#include <exception>

//Yeni eklendi
class Nec{
public:
	auto operator co_await()
	{
		 std::cout << "Nec::operator co_await()\n";
		 return std::suspend_always();
	}
};

class Corapi
{
public:
	struct promise_type;
	using CrtHandle = std::coroutine_handle<promise_type>;
	Corapi() = default;
	Corapi(const Corapi&) = delete;
	Corapi(const CrtHandle&h) : m_h{ h } { };
	Corapi& operator=(const Corapi&) = delete;
	~Corapi()
	{
		if(m_h)
			m_h.destroy();
	}

	bool resume()const
	{
		if (!m_h || m_h.done())
		{
			return false;
		}

		m_h.resume();
		return !m_h.done();
	}
private:
	CrtHandle m_h = nullptr;
};


//Promisetype.jpg de member funcları var
// Burada zorunlu olanlar var.Promise_type.jpg de var.
struct Corapi::promise_type		  //sınıf içidne de yazabilirdik.
{
	auto get_return_object()noexcept
	{
		return Corapi{ CrtHandle::from_promise(*this) };
	}

	auto initial_suspend()
	{
		return std::suspend_always{}; //Dİkkat !!!
	}

	auto final_suspend()noexcept
	{
		return std::suspend_always{}; //Dİkkat !!!
	}

	void unhandled_exception()
	{
		return std::terminate();
	}

	void return_void()
	{
		//bos
	}

};


Corapi cprint(std::string name)
{
	std::cout << name << " hosgeldiniz \n";
	co_await Nec{};										// DİKKAT!!!

	std::cout << "tanistigimiza memnun oldum ?\n";
}

int main()
{
	auto coreapi = cprint("Harun");

	std::cout << "Resume oncesi\n";
	int cnt{};
	while (coreapi.resume())  // promise type yazılmazsa resume çalışmıyor zaten.Bunu Corapi içine yazıyoruz
	{
		std::cout << "count = " << ++cnt << '\n';
	}

}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ÖR:

Eğer co_yield kullanacaksak, bunu handle etmek için yield value funcına ihtiyaç var.
Çünkü co_yield, yield_value funcına çağrıya dönüşüyor. 
yield_value nun return değeri awaitable türü olmalı yine.

co_return kullansaydık az önce return değeri olmadığı için sınıfa return void yazmamız yetti.
Co_returnü ifadede kullanacaksak sınıfın return value funcı olmalı.

Bir öncekişnden farkı her suspensionda bir değer iletecek.

class Generator{
public:
	struct promise_type;
	using CoroHandle = std::coroutine_handle<promise_type>;

private:
	CoroHandle m_handle;

public:
	struct promise_type{
		int m_coro_val{0};
		auto yield_value(int val) //Burası önemli !!!
		{
			std::cout << "Promise_type::yield_value(int val) val = " << val << "\n";
			m_coro_val = val;
			return std::suspend_always{};
		}

		auto get_return_object(){ return CoroHandle::from_promise(*this);}
		auto initial_suspend(){ return std::suspend_always{};}
		void return_void() { }
		void unhandled_exception(){ std::terminate();};
		auto final_suspend() noexcept{ return std::suspend_always{}; }; 
	};

	Generator(auto handle) : m_handle{ handle } { }
	~Generator()
	{
		if(m_handle)
			m_handle.destroy();
	}
	
	Generator(const Generator&) = delete;
	Generator& operator=(const Generator&) = delete;

	bool resume() const
	{
		if(!m_handle ||m_handle.done())
		{
			return false;
		}

		m_handle.resume();
		return !m_handle.done();
	}

	int get_value()const
	{
		return m_handle.promise().m_coro_val; 
	}

};

Generator coro(int max)
{
	std::cout << "			CORO " << max << " start\n";

	for(int val = 1; val <= max; ++val)
	{
		std::cout << "			CORO " << val << '/' << max << '\n';
		
		co_yield val;		 // Suspend with value
		// co_yield aşağıdakine dönüşüyor
		// co_await yield_value() ya dönüştrüyor.
	}


	std::cout << "			CORO " << max << "end\n";

}

int main()
{
	auto gen = coro(3);

	std::cout << "coro() started\n";

	while(gen.resume())
	{
		auto val = gen.get_value();
		std::cout << "coro() suspended with " << val << '\n';
	}

}


OUTPUT
------
coro() started
						CORO 3 start
						CORO 1/3
Promise_type::yield_value(int val) val = 1
coro() suspended with 1
						CORO 2/3
Promise_type::yield_value(int val) val = 2
coro() suspended with 2
						CORO 3/3
Promise_type::yield_value(int val) val = 3
coro() suspended with 3
						CORO 3end










































*/