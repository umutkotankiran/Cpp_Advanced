/*

Gruptan sorduğu soruyu cevapladı hoca

Bugün yeni bir araç tanıtılacak.

COROUTINES
----------
İlk önce github notlarını koyuyorum.

NECATI ERGIN GITHUB NOTLARI
---------------------------
Normal fonksiyonlar çağrıldığında kodlarının tamamı çalıştırılır. Fonksiyonun çalışmasının sonlanması için
- Kodlarının tamamının çalıştırılması (void fonksiyonlar)
- Bir return deyiminin yürütülmesi
- Fonksiyonun kodundan bir exception gönderilmesi yoluyla çıkılması gerekir.

------------------------------------------------------------------------------------------------------------------------------------------------

Oysa coroutine'ler kodları birden fazla adımda çalışabilen fonksiyonlardır. Belirli anlarda, bir coroutine'in çalışmasını askıya alabiliriz, 
yani çalışmasını daha sonra tekrar başlatmak üzere durdurabiliriz. (İngilizcede bu anlamda suspend fiili kullanılıyor, isim hali suspension)
Sonra çalışmasını tekrar devam ettirebiliriz. Bir coroutine fonksiyonunun çalışmasını örneğin,

- bir olayı beklemesi gerektiği için,
- yapılacak başka (daha önemli) işler olduğu için,
- koda iletmemiz gereken bir ara sonuç olduğu için, daha sonra tekrar başlatmak üzere durdurabiliriz.

------------------------------------------------------------------------------------------------------------------------------------------------

Bu nedenle bir coroutine'in başlatılması, bir kısmı tamamlanıncaya kadar çalıştırılmak üzere bir fonksiyonun çalışmasının başlatılması anlamına gelir. 
Çağıran fonksiyon ve coroutine, iki yürütme yolu arasında gidip gelerek çalışır. Burada iki fonksiyonun kodu paralel olarak çalıştırılmamaktadır. 
Kontrol akışını ayarlayarak adeta ping-pong oynanmaktadır.

- Bir fonksiyon, bir coroutine'in mevcut kontrol akışını başlatmaya ya da devam ettirmeye coroutine'in deyimlerini başlatarak ya da devam ettirerek karar verebilir.
- Daha sonra bir coroutine çalıştığında, coroutine yürütmesini duraklatmaya veya sonlandırmaya kendisi karar verebilir.
  Bu da coroutine'i başlatan veya devam ettiren fonksiyonun kontrol akışına devam ettiği anlamına gelir.
- Bir coroutine'in en basit şeklinde, hem ana kontrol akışı hem de coroutine'in kontrol akışı aynı thread'de çalışır.
- Birden fazla thread kullanmak zorunda değiliz ve eşzamanlı erişimle (synchronosied access) uğraşmak zorunda değiliz. 
  Ancak, coroutine'leri farklı thread'lerde çalıştırmak mümkündür.
- Hatta bir coroutine'i daha önce askıya alındığı yerden farklı bir thread'de devam ettirebiliriz. Coroutine'ler ortogonal bir özelliktir,
  ancak birden fazla thread birlikte kullanılabilir. -bir coroutine kullanmak, arka planda zaman zaman başlattığınız ve devam ettirdiğimiz bir
  fonksiyona sahip olmak gibidir. Bununla birlikte, bir coroutine'in ömrü iç içe geçmiş kapsamların ötesine geçtiğinden, 
  bir coroutine aynı zamanda durumunu bir bellekte saklayan ve bununla başa çıkmak için bir API sağlayan bir nesnedir.

------------------------------------------------------------------------------------------------------------------------------------------------

Bir fonksiyonun coroutine olarak ele alınabilmesi için tanımmında aşağıdaki anahtar sözcüklerden en az birine sahip olması gerekir.
- co_await
- co_yield
- co_return

Bir coroutine içinde bu anahtar sözcüklerden hiçbirinin gerekli olmaması durumunda, sonunda açıkça bir
- co_return; 
deyimi yazmamız gerekir.

------------------------------------------------------------------------------------------------------------------------------------------------

Coroutine'ler bir stack yapısında değiller (stackless coroutine). Bir coroutine'i bir coroutine içinden çalıştırıp onu çalıştıran coroutine'i suspend etmeden, suspend edemeyiz.
coroutine fonksiyonlar için aşağıdaki kısıtlamalar söz konusudur:

- main fonksiyonu bir coroutine olamaz.
- variadic bir fonksiyon bir coroutine olamaz.
- Sınıfların constructor'ları coroutine olamaz.
- Bir sınıfın destructor'ı coroutine olamaz.
- coroutine bir constexpr fonksiyon olamaz.
- coroutine bir consteval fonksiyon olamaz.
- coroutine'ler auto return type kullanamazlar.
- coroutine'ler normal return deyimlerini kullanamazlar.
- Bir coroutine genellikle çağıran için coroutine arayüzü (coroutine interface) olarak hizmet veren bir nesne döndürür. 
coroutine'in amacına ve kullanımına bağlı olarak, bu nesne zaman zaman bağlamı askıya alan veya değiştiren çalışan bir görevi,
zaman zaman değer üreten bir üreteci veya tembel olarak (lazily) ve talep üzerine bir veya daha fazla değer döndüren bir fabrikayı temsil edebilir.

------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------

Coroutine Frame
---------------

Bir coroutine başlatıldığında şunlar olur:
Coroutine için gerekli bütün verilerin tutulması için bir coroutine frame oluşturulur. Bu bellek alanı genel olarak heap'ten edinilir. Ancak derleyiciler, 
yapabiliyorlar ise coroutine frame'i stack üzerinde de oluşturabilirler. Bu optimizasyon tipik olarak coroutine'in yaşam süresinin 
çağıran kodun yaşam süresinin içinde kaldığı ve derleyicinin coroutine frame için ne kadar bellek alanı gerektiğini hesaplayabileceği durumlarda yapılabilir.
coroutine'in bütün parametreleri coroutine frame'e kopyalanır. Referansların kopyalanması referans semantiği ile olur. Yani değerler kopyalanmaz.
Coroutine'in çalışması devam ettiği sürece referanslar geçerli durumda kalmalıdır. Geçersiz (dangling) referanslar oluşabileceğinden 
referans parametrelerden genel olarak kaçınmak gerekir. promise nesnesi coroutine frame içinde oluşturulur. 
Bu nesnenin görevi coroutine'in durum bilgisini tutmak ve coroutine çalışırken kullanılacak özelleştirme (customization) noktaları sağlamaktır.
Bir promise nesnesi yoluyla coroutine'in durumu (state) kontrol edilebilir ve takip edilebilir.

Bir coroutine genellikle çağıran için coroutine arayüzü (coroutine interface) olarak hizmet veren bir nesne döndürür. 
coroutine'in amacına ve kullanımına bağlı olarak, bu nesne zaman zaman bağlamı askıya alan veya değiştiren çalışan bir görevi,
zaman zaman değer üreten bir üreteci veya tembel olarak (lazily) ve talep üzerine bir veya daha fazla değer döndüren bir fabrikayı temsil edebilir.

coroutine interface coroutine'in geri dönüş değeri türüdür. Coroutine'i çağıran kod coroutine ile iletişimini, 
(örneğin coroutine'in çalışmasını devam ettirmek gibi) coroutine interface'i kullanarak gerçekleştirir. Derleyicinin ürettiği kod coroutine interface'in
belirli öğelerini kullandığı için coroutine interface türünün belirli öğelere sahip olması zorunludur. Aksi halde derleme zamanında hata oluşur.

coroutin interface'in bir sınıf türünden olması zorunludur. Çünkü derleyicinin ürettiği kodda bu sınıfın bazı üye fonksiyonları ve bir
üye türü (nested type) kullanılmaktadır. Örneğin bir coroutine'in geri dönüş türü int türü olamaz. coroutine'in kendisine çağıran koda 
ilettiği yani ürettiği değerin türü ile coroutine'in geri dönüş türü birbiriyle karıştırılmamalıdır.

Derleyici bir coroutine için ürettiği kodda coroutine interface'in promise_type isimli içsel türünü (nested type) kullanır. 
Bu yüzden coroutine interface'in öyle ya da böyle promise_type isimli bir nested type'a sahip olması gerekir.
Bunu sağlamanın birden fazla yolu olabilir. Yine derleyici tarafından yazılan kodlar belirli durumlarda promise_type'ın bazı üye fonksiyonlarını çağırmaktadır. 
Bu yüzden promise_type'ın belirli üye fonksiyonlarının da tanımlanması gerekmektedir. coroutine'in gerçekleştirilmesinde önemli rol oynayan iki tür var:

------------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------------

Promise Türü (promise_type)
---------------------------
promise_type, coroutine’in davranışını (başlama, bitirme, yield etme, return etme) tanımlayan türdür. 
Bu türden bir nesne coroutine frame içinde derleyici tarafından oluşturulur. promise nesnesini derleyicinin ürettiği kod oluşturur. 
Bir coroutine çağrıldığında coroutine frame edinilir edinilmez promise_type nesnesi oluşturulur. Bu tür, coroutine'in kullanılmasında customization noktaları sunar.
Bu tür kullanılanarak coroutine'in belirli noktalarda nasıl davranacağını belirlenebilir. Derleyicinin ürettiği kodlar belirli noktalarda promise_type'ın

get_return_object()
-------------------
initial_suspend(), -_ return_value()_ gibi fonksiyonları ı çağırır. Yani bu türün üye fonksiyonları belirli durumlarda çağrılan callback'ler olarak kullanılır.
Bu türden bir nesnenin içinde coroutine'in çağıran kod ile veri alış verişini sağlamak için değişkenler tutulabilir.

std::coroutine_handle<>
-----------------------
coroutine başlatıldığında promise sınıfının üye fonksiyonlarından biri kullanılarak std::coroutine_handle türünden bir nesne oluşturulur.
Bu tür coroutine'in kontrolü için alt seviyeli bir arayüz sağlar. coroutine_handle nesnesi kullanılarak coroutine_frame'de tutulan promise_type nesnesine erişilebilir.
coroutine_handle nesnesi kullanılarak şunlar yapılabilir:

coroutine'in çalışması devam ettirilebilir: (resume)
coroutine state (coroutine frame) destroy edilebilir. coroutine_state' te tutulan promise_type nesnesine erişilebilir.

coroutine handle bir "callable object" olarak kullanılabilir.
coroutine handle bir "nullable" türdür. nullptr değerinde olabilir. henüz bir coroutine frame'i göstermeyen coroutine_handle değişkeni nullptr değerindedir. 
Sınıfın operator bool işleviyle ya da nullptr sabitiyle karşılaştırma ile bu durum sınanabilir:

if (crt_handle) { 
	//...
}

if (crt_handle != nullptr) {
	//...
}

------------------------------------------------------------------------------------------------------------------------------------------------

Standart kütüphane coroutine_handle türünü generic bir tür olarak sunar:

std::coroutine_handle<>
// ya da
std::coroutine_handle<void>
ya da

std::coroutine_handle<promise_type>

------------------------------------------------------------------------------------------------------------------------------------------------

std::coroutine_handle<void> türünden bir nesneye herhangi bir türden coroutine_handle nesnesi atanabilir. (void pointer türüne diğer pointer türlerinden atama yapılabilmesi gibi.)
coroutine_handle nesnesi fonksiyon çağrı operatörünün operandı olduğunda coroutine'in durdurulduğu noktadan çalışmaya tekrar devam etmesini sağlar.

coroutine'in geri dönüş türü olan coroutine interface bu türlere erişim olanağı sunar:

- Nasıl bir promise türü kullanılacak? promise türü tipik olarak bir içsel tür (nested type) yapılır.
- coroutine_handle nesnesi nerede saklanacak? Tipik olarak coroutine_handle coroutine interface sınıfının bir veri elemanı yapılır.
- coroutine_interface türü coroutine'in kullanılabilmesi için müşteri kodlara kontrol olanağı veren bir arayüz sağlar. 
- Standart kütüphanenin sağladığı coroutine_handle<> türü çalışmakta ya da durdurulmuş olan bir coroutine'e erişmek için kullanılır. 
- Template parametresi olan tür coroutine'in promise_type türüdür. Bu türde çeşitli değerler tutan veri elemanları ya da çeşitli operasyonlar sağlayan üye fonksiyonlar bulunabilir.

------------------------------------------------------------------------------------------------------------------------------------------------

coroutine_handle<> türünün üye fonksiyonlarına bakalım:
coroutine_handle<promise_type>::from_promise(prm)
promise nesnesinden bir handle oluşturur.

- CoroHandleType{}
  default constructor. Henüz bir coroutine ile ilişkilendirilmemiş bir handle oluşturur.

- CoroHandleType{nullptr}
  Henüz bir coroutine ile ilişkilendirilmemiş bir handle oluşturur.

- CoroHandleType{hdl}
  Bir handle'dan yeni bir handle nesnesini oluşturur. Bu durumda her iki handle da aynı coroutine'e işaret etmektedir.

- hdl = hdl2
  Bir handle'a yeni bir handle nesnesini atar. Bu durumda her iki handle da aynı coroutine'e işaret etmektedir.

- if (hdl)
  handle'ın bir coroutine'i gösterip göstermediğini sınar.

- ==, !=
  iki handle'ın aynı coroutine'i gösterip göstermediğini sınar.

- <, <=, >, >=, <=>
  handle'ların sıralanması için gerekli karşılaştırma işlemlerini sağlar.

- hdl.resume()
  coroutine'in durdurulduğu noktadan tekrar çalıştırılmasını (resume) sağlar.

- hdl() (function call operator function)
  coroutine'in durdurulduğu noktadan tekrar çalıştırılmasını (resume) sağlar.

- hdl.done()
  coroutine'in çalışmasının tamamlanmış olup olmadığını sınar. Bu fonksiyon true değer döndürür ise artık ilgili crt için resume fonksiyonu çağrılmamalıdır.

- hdl.destroy()
  coroutine nesnesini sonlandırır.

- hdl.promise()
  coroutine'in promise nesnesine eriştirir.

- hdl.address()
  coroutine verilerinin tutulduğu içsel adresi döndürür.

- coroutine_handle::from_address(addr)
  argüman olan adresle ilişkili bir handle nesnesi oluşturur.

------------------------------------------------------------------------------------------------------------------------------------------------

Sınıfın static üye fonksiyonu olan from_promise() bir coroutine'den bir handle oluşturması için çağrılır. 
Bu fonksiyon aslında promise nesnesinin adresini handle nesnesi içinde saklar. prm bir promise nesnesi olmak üzere aşağıdaki gibi çağrılabilir:

auto hdl = std::coroutine_handle<decltype(prm)>::from_promise(prm);
from_promise() tipik olarak promise_type sınıfının get_return_object fonksiyonu içinde çağrılır:

class CoroFace{
public:
    struct promise_type {
        auto get_return_object() 
        { 
            return CoroFace{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        //...
    };
    //...
};
default ctor. ya da std::nullptr_t parametreli ctor. ile oluşturulan bir handle nesnesi boştur, hiçbir coroutine'i göstermez.
Bu durumdaki bir handle nesnesi için çağrılan operator bool fonksiyonu false değer döndürür.

std::coroutine_handle<promise_type> hdl1{};
std::coroutine_handle<promise_type> hdl2{nullptr};

if (hdl) // false
handle nesnelerinin birbirine kopyalanması ya da atanması maliyeti düşüktür. Kopyalanan handle nesnesinin sarmaladığı bir pointer değişkendir.
Bu yüzden coroutine_handle fonkiyonlara tipik olarak call by value olarak gönderilir. Kopyalamaya izin verildiği için birden fazla 
handle aynı coroutine'i gösterebilir. Eğer başka bir handle coroutine'i yok etmişse handle'ın resume ya da destroy 
fonksiyonlarını çağırmak tanımsız davranıştır (dangling pointer).

Sınıfın address() fonksiyonu sarmalanan pointer değişkenin değerini void* türden bir adres olarak döndürür. 
Böylece bu fonksiyondan elde ettiğimiz pointer değişkeni başka bir bağlamda kullanarak o bağlamda sınıfın from_address isimli 
static üye fonksiyonuna çağrı yaparak aynı coroutine'i gösteren bir başka handle nesnesi oluşturabiliriz:

    auto handle1 = std::coroutine_handle<decltype(prm)>::from_promise(prm);
    //...
    void* handle_ptr = handle.address();
    //...
    auto handle2 = std::coroutine_handle<decltype(prm)>::from_address(handle_ptr);
    handle1 == handle2 // true
coroutine yok edildikten sonra bu adres kullanılmamalıdır.

Tüm coroutine_handle türlerinden std::coroutine sınıfı türüne örtülü tür dönüşümü vardır.

namespace std {
	template<typename Promise>
	struct coroutine_handle {
	//...
	// implicit conversion to coroutine_handle<void>:
	constexpr operator coroutine_handle<>() const noexcept;
	//...
	};
}

------------------------------------------------------------------------------------------------------------------------------------------------

İlk standarda göre tüm coroutine_handle türlerinin std::coroutine türünden kalıtım yoluyla elde dilmesi gerekiyordu. Bu durum http://wg21.link/lwg3460 ile değiştirildi.

promise nesnesi coroutine'in durumunu yöneten, coroutine'in davranışını belirleyen ve coroutine'in durumunun izlenmesine olanak sağlayan nesnedir.
coroutine'in yürütülmesi sırasında belirli noktalarda ilgili promise nesnesinin belirli üye fonksiyonları çağrılır. 
promise arayüzünde coroutine'in davranışını belirleyen üye fonksiyonlar bulunur. coroutine kütüphanesinin yazarı, 
coroutine çağrıldığında ne olacağını, coroutine'in çalışması sonlandığunda (doğal yolla ya da işlenmeyen bir exception durumunda) ne olacağını belirler.
Ayrıca coroutine içinde bulunan co_await ve co_yield ifadelerinin davranışlarını belirler. 
Coroutine fonksiyon her çağrıldığında coroutine frame içinde bir promise nesnesi oluşturulur.

Bazı detayları şimdilik göz ardı edersek, bir coroutine çağrıldığında aşağıdaki kodların çalıştığını düşünebiliriz:
1- operator new fonksiyonun çağrılmasıyla coroutine frame için bir bellek alanı elde edilir. (Burada, derleyici koşullar uygunsa dinamik bir bellek alanının kullanılmaması biçiminde bir optimizasyon gerçekleştirebilir.)
2- Fonksiyon parametreleri coroutine frame'e kopyalanır.
3- promise_type türünden promise nesnesinin oluşturulması için promise_type sınıfının ilgili constructor'ı çağrılır.
4- promise nesnesinin get_return_object fonksiyonu çağrılarak coroutine fonsiyonun geri dönüş değeri elde edilir. Sonuç bir yerel değişkende saklanır ve coroutine ilk suspend edildiğinde bu değer çağıran koda iletilir.
5- promise nesnesinin initial_suspend() üye fonksiyonu çağrılır. Bu fonksiyonun geri dönüş değeri co_await operatörünün operandı yapılarak co_await deyimi yürütülür.
6- co_await promise.initial_suspend() ifadesi yürütüldükten sonra sonucu coroutine resume edildiğinde coroutine gövdesine yazılan tüm kodlar çalıştırılır.


Programın akışı co_return deyimine geldiğinde ise şunlar olur:
1- promise nesnesinin return_void() ya da return_value() fonksiyonu çağrılır.
2- Tüm yerel değişkenlerin hayatı hayata geldikleri sırayla ters sırada sonlandırılır.
3- promise nesnesinin final_suspend() fonksiyonu çağrılır ve geri dönüş değeri co_await operatörünün operandı yapılır.


promise_type türü
- Bu tür, bir coroutine ile işlem yapmak için belirli "customization" noktalarını tanımlamak ve oluşturmak için kullanılır.
- Belirli durumlarda çağrılan call_back fonksiyonları tanımlar.
- coroutine'in dönüş değerinin nasıl oluşturulacağını veya alınacağını tanımlar (genellikle coroutine handle'ın oluşturulmasını da sağlar.)
- coroutine'in çalışmasının başında ya da sonunda suspend edilip edilmeyeceğini (durdurulup durdurulmayacağını) belirler.
- coroutine'i çağıran kod ile coroutine arasındaki veri alış verişini sağlar.
- İşlenmemiş hata nesneleriyle (unhandled exception) ilgili yapılacakları belirler.

------------------------------------------------------------------------------------------------------------------------------------------------

promise_type'ın implementasyonu
Derleme zamanında bir hata olmaması için promise_type sınıfının aşağıdaki fonksiyonları tanımlaması gerekmektedir. (çağrıldıkları sıra ile)

Constructor
----------
Bir coroutine başlatıldığında promise_type nesnesini oluşturmak için coroutine frame tarafından promise sınıfının constructor'ı çağrılır.
Bu yüzden promise_type'ın geçerli olarak çağrılabilir bir constructor'a sahip olması gerekir. Basit senaryolar için sınıfın default ctor'ı yeterli olabilir.
Constructor, derleyici tarafından coroutine'i bazı argümanlarla başlatmak için kullanılabilir. 
Bunun için, constructor imzasının, çağrıldığında coroutine'e aktarılan argümanlarla eşleşmesi gerekir. Bu teknik özellikle coroutine_traits tarafından kullanılır.

get_return_object()
-------------------
Bu fonksiyon derleyicinin ürettiği kod tarafından coroutine arayüzünü oluşturması için çağrılır.
Derleyicinin ürettiği kodda bu fonksiyonun çağrılmasıyla coroutine arayüz nesnesi oluşturulur.
Oluşturulan nesne coroutine'i çağıran koda coroutine'in geri dönüş değeri ile iletilir.
coroutine arayüz nesnesi tipik olarak std::coroutine_handle sınıfının static üye fonksiyonu olan from_promise tarafından oluşturulur.

initial_suspend()
-----------------
Bu fonksiyon, coroutine başlatıldıktan hemen sonra ilk kez suspend edilsin mi sorusunun cevabını verir. 
Derleyicinin ürettiği kod tarafından, prm promise nesnesi olmak üzere, aşağıdaki gibi çağrılır:

co_await prm.initial_suspend()
------------------------------
Bu yüzden fonksiyonun geri dönüş türü bir "awaitable" tür olmalıdır.
Eğer coroutine'in başlar başlamaz ilk kez suspend edilmesini isteniyorsa bu fonksiyonun geri dönüş değeri std::suspect_always olmalıdır.

struct promise_type {
	//...
	auto initial_suspend() 
	{ 
		return std::suspend_always{}; 
	}
	//...
}

------------------------------------------------------------------------------------------------------------------------------------------------

Eğer coroutine'in başlar başlamaz ilk kez suspend edilmesi istenmiyor ise bu fonksiyonun geri dönüş değeri std::suspect_never olmalıdır.

struct promise_type {
	//...
	auto initial_suspend() 
	{ 
		return std::suspend_never{}; 
	}
	//...
}

Tabi bu fonksiyon, coroutine başlatıldığında suspend edilip edilmeyeceği kararının çalışma zamanında değerlendirilen koşullara göre verilmesini de sağlayabilir. 
Yine bu fonksiyon içinde promise_type nesnesinin veri elemanlarına atama yapılabilir.

final_suspend() noexcept
------------------------
Bu fonksiyon coroutine'in son kez suspend edilebileceği noktayı belirler ve derleyicinin ürettiği kodda aşağıdaki gibi çağrılır:
prm bir promise_type nesnesi olmak üzere

co_await prm.final_suspend()
----------------------------
Bu fonksiyon coroutine frame tarafından coroutine gövdesini içine alan try bloğunun dışında ve return_void(), return_value() ya da unhandled_exception() 
fonksiyonları çağrıldıktan sonra çağrılır. Bu yüzden noexcept olmalıdır. Bu üye fonksiyonun adı biraz yanıltıcı olabilir. 
Programın akışı coroutine'in sonuna ulaştıktan sonra bir kez daha yeniden başlatmaya zorlamak için std::suspend_never{} döndürülebileceği gibi yanlış bir izlenimini verebilir.
Ancak, son suspension noktasında gerçekten durdurulmuş bir coroutine'i devam ettirmek tanımsız davranıştır. Bu fonksiyonda durdurulmuş bir coroutine ile
yapabileceğiniz tek şey onu "destroy" etmektir. Bu nedenle, bu üye fonksiyonun gerçek amacı, üretilen bir sonucu aktarmak, tamamlanmış olduğu sinyalini vermek veya
başka bir yerde bir devamı devam ettirmek gibi bazı mantıkları yürütmektir. coroutine'lerin mümkün olduğunca bu noktada durdurulacak şekilde yapılandırılması önerilir.

Bu nedenle, aksi yönde bir karar almak için bir nedeniniz yoksa, final_suspend() fonksiyonu her zaman std::suspend_always{} döndürmelidir. Örneğin:

struct promise_type {
	//...
	auto final_suspend() noexcept 
	{ 
		//...
		return std::suspend_always{};
	}
};


unhandled_exception()
---------------------
co_yield veya co_return'ün kullanılıp kullanılmadığına ve nasıl kullanıldığına bağlı olarak, aşağıdaki fonksiyonlardan 
bazılarının promise sınıfı tarafından tanımlanması gerekir. coroutin'in sonunda çağrılmak üzere aşağıdaki fonksiyonlardan birinin tanımlanması gerekir:

return_void()
-------------
coroutine çalışmasının sonucunda çağıran koda bir değer üretmeyecek ise bu fonksiyonun tanımlanması gerekir.
coroutine sona ulaştığında (gövdesinin sonuna ulaştığında ya da argümansız bir co_return ifadesine ulaştığında) çağrılır.

return_value()
--------------
coroutine çalışmasının sonucunda çağıran koda bir değer üretecek ise bu fonksiyonun tanımlanması gerekir. 
coroutine bir argümanlı bir co_return ifadesine ulaşırsa çağrılır. Aktarılan argüman belirtilen türde olmalı veya bu türe dönüştürülmelidir.
coroutine'nin geri bir noktada geri dönüş değeri üretmesi fakat bir başka sonlanma noktasında geri dönüş değeri üretmemesi tanımsız davranıştır.

ResultType corofunc( xxx ) // xx nedir bakmak lazım. Olmayadabilir
{
	if ( xxx ) {
		co_return 42;
	}
}
Yukarıdaki coroutine geçerli değildir. Hem return_void() hem de return_value() fonksiyonunun çağrılması doğru değildir. üstelik

return_value(int);
gibi bir fonksiyonun tanımlanması durumunda derleyici yukarıdaki kod için bir bir uyarı mesajı da vermeyebilir.

------------------------------------------------------------------------------------------------------------------------------------------------

Farklı imzaya sahip birden fazla return_value fonksiyonu tanımlanabilir yani return_value fonksiyonu overload edilebilir. 
Hatta return_value fonksiyonu generic olarak da tanımlanabilir.

struct promise_type {
	//...
	void return_value(int val) 
	{ 
		//...	
	}

	void return_value(std::string val) 
	{ 
		//...
	}
};

Bu durumda coroutine birden fazla ve farklı türden ifadelere sahip co_return statement'a sahip olabilir.

ReturnType corofunc()
{
	int val{0};
	if ( xxx ) 
	{
		co_return "error";
	}
	
	co_return val;
}

yield_value(Type)
-----------------
Coroutine içinde programın akışı bir co_yield deyimine geldiğinde yield_value fonksiyonu çağrılır. Bu fonksiyonun geri dönüş değeri co_await operatörünün operandı yapılır:

struct promise_type {
	//...
	auto yield_value(int val) 
	{ 
		return std::suspend_always{}; // - suspend coroutine
	}

	auto yield_value(std::string val) 
	{ 
		return std::suspend_always{}; // - suspend coroutine
	}
};

------------------------------------------------------------------------------------------------------------------------------------------------

co_await ifadesi ne yapar ve awaitable bir tür ne anlama gelir?
co_await anahtar sözcüğü unary bir operatör olarak görev yapar. Yani tek bir operand alır.
co_await operatörünün operandı olan nesnenin bazı gereklilikleri yerine getirmesi gerekir.
coroutine fonksiyonumuzda co_await operatörünü kullandığımızda, bizim için hazır olabilecek veya olmayabilecek bir şeyi beklediğimizi ifade etmiş oluruz.
Eğer beklediğimiz şey her ne ise hazır değilse, co_await o anda çalışmakta olan coroutine'i askıya alır, 
yani onun çalışmasını durdurur ve kontrolü coroutine'i çağıran koda geri verir. Asenkron görev tamamlandığında, 
çağıran fonksiyon, kontrolü başlangıçta görevin bitmesini bekleyen coroutine'e geri aktarmalıdır.

Aşağıdaki gibi bir ifade olsun:

co_await X{};
------------
Bu kodun derlenebilmesi için X'in awaitable bir tür olması gerekir.

awaitable nedir?
----------------
Derleyici co_await operatörünün operandı olan ifadeden yola çıkarak bir awaiter nesnesi oluşturmak zorundadır. Oluşturamaz ise sentaks hatası olur.

Derleyici önce promise_type sınıfının await_transform isimli bir fonksiyonu olup olmadığını kontrol eder. 
Eğer promise_type sınıfının await_transform isimli bir fonksiyonu varsa derleyici co_await operatörünün operandı olan ifadeyi 
bu fonksiyona argüman olarak göndererek bu fonksiyonu çağırır. Bu fonksiyonun geri dönüş değerini awaitable olarak kullanır. 
Yoksa co_await operatörünün operandı olan ifade doğrudan awaitable olarak kullanılır.

Bu kez derleyici buradan elde edilen awaitable olarak kullanılacak sınıfın üye operator co_await fonksiyonunun
bulunup bulunmadığına bakar. Eğer awaitable sınıfın böyle bir fonsiyonu varsa derleyici bu fonksiyonun geri dönüş değerinden awaiter nesnesini oluşturur. 
Eğer böyle bir üye fonksiyon yok ise derleyici bu kez uygun bir parametreye sahip global operator co_await fonksiyonunun olup olmadığına bakar. 
Varsa bu fonksiyona awaitable nesnesini argüman olarak gönderir. Böyle bir fonksiyon yok ise bu durumda derleyici doğrudan awaitable nesnesini kullanır. 
(bu durumda awaitable nesnesi bir awaiter olmak zorundadır.)

Standart kütüphane bize temel düzeyde
std::suspend_always ve std::suspend_never awaiter sınıflarını hazır olarak sunmaktadır.
std::suspend_never : co_await operatörünün coroutine'i durdurmamasını (askıya almamasını) sağlar.

awaiter nedir?
--------------
Aşağıda listelenen üç üye işlevi doğrudan gerçekleştiren veya alternatif olarak bu üye işlevlerle bir nesne üretmek için co_await() operatör fonksiyonunu tanımlayan herhangi bir tür, awaiter bir türdür:

await_ready()
-------------
await_ready fonksiyonu sonucun hazır olup olmadığını (true) veya geçerli coroutine'i askıya alıp sonucun hazır olmasını beklemek 
gerekip gerekmediğini ifade eden bool türden bir değer döndürür. Bu fonksiyonun coroutine askıya alınmadan (durdurulmadan) hemen önce çağrılır.

Askıya almayı (geçici olarak) tamamen kapatmak için kullanılabilir.
await_ready fonksiyonu true döndürürse askıya alma isteği reddedilmiş demektir. Yani fonksiyonun true değer döndürmesi coroutine'i durdurmadan devam etmeye "hazırız" anlamına gelir.
Genellikle, bu fonksiyon yalnızca false değer döndürür ("hayır, herhangi bir durdurma işleminden kaçınmayın/engellemeyin anlamında").
Ancak bu fonksiyon bir koşula bağlı olarak olarak true değer döndürebilir (örneğin, coroutine'in durdurulması bazı verilerin mevcut olmasına bağlıysa).

[Geri dönüş türüyle await_suspend() fonksiyonu da , coroutine'in askıya alınmasını kabul etmeme sinyali de verebilir 
(true ve false geri dönüş değerlerinin burada zıt anlama sahip olduğuna dikkat edin: await_suspend() 
fonksiyonunun true değer döndürmesi ile askıya alma kabul edilmiş olur.]

await_ready() fonksiyonu ile askıya almayı kabul etmemek, programın coroutine'in askıya alınmasını başlatma maliyetinden tasarruf edilmesini sağlar.
Bu fonksiyonun kodu çalışırken coroutine'in henüz durdurulmamıştır. Yani bu fonksiyon içinde resume()
ya da destroy() işlevleri (dolaylı olarak) çağrılmamalıdır. Bu fonksiyon içinde askıya alınan coroutine için resume()
veya destroy() çağrısı yapılmadığından emin olunduğu sürece daha karmaşık işleri gerçekleştirecek fonksiyonlar bile burada çağrılabilir.

await_suspend (coroutine_handle)
--------------------------------
await_ready() fonksiyonu false döndürürse, bu fonksiyon co_await'i çalıştıran coroutine'in handle'ı ile çağrılır.
Bu fonksiyon bize bir asenkron kodun çalışmasını başlatma ve ilgili görev bittiğinde tetiklenecek bir bildirim için abone olma ve ardından coroutine'i devam ettirme fırsatı verir.

await_suspend fonksiyonu, coroutine askıya alındıktan hemen sonra çağrılır. Fonksiyon parametresi olan coroutine_handle durdurma işleminin talep edildiği coroutine'e erişimi sağlayan handle'dır.
Bekleyen coroutine'in handle'ının türündendir: std::coroutine_handle.
Burada, durdurulan coroutine'in veya bekleyen bir coroutine'in çalışmasını hemden devam ettirmek ve diğerini daha sonra devam ettirmek için zamanlamak
da dahil olmak üzere bir sonraki adımda ne yapılacağı belirlenebilir. Bunu desteklemek için farklı geri dönüş türleri kullanılabilir (bu durum aşağıda ele alınacaktır).

Hatta burada coroutine destroy edilebilir.
Ancak bu durumda coroutine'in başka bir yerde kullanılmadığından emin olmanız gerekir (örneğin bir coroutine arayüzünde done() çağrısı yapmak gibi).
await_suspend() işlevinin geri dönüş türü ise şunlar olabilir:

geri dönüş türünün void olması await_suspend() fonksiyonun geri dönüş türü void olursa fonksiyon içindeki 
deyimlerin yürütülmesinden sonra sonra coroutine'in çalışması durdurulur ve programın akışı coroutine'i çalıştıran koda aktarılır.

geri dönüş türünün bool olması await_suspend() fonksiyonun geri dönüş türünün bool olması ile coroutine'in durdurulması
bir koşula bağlanabilir. Fonksiyonun false döndürmesi coroutine'in durdurulmayacağı anlamına gelir. (await_ready() işlevinin bool dönüş değerlerinin tersidir).

geri dönüş türünün coroutine_handle olması await_suspend() fonksiyonu başka bir coroutine'in handle'ını döndürebilir.
Bu durumda coroutine'in çalışması durdurulur ancak programın akışı doğrudan coroutin'i çalıştıran koda aktarılmaz. 
handle'ı döndürülen coroutine çalıştırılır. Bu coroutine de programın akışını bir başka coroutin'e yönlendirebilir.
Kontrolün coroutine'i çalıştıran koda geri dönmesi için coroutine'in akışın yönlendirileceği başka bir coroutine belirlememesi gerekir.

Peki, programın akşı bir koşula bağlı olarak bir başka coroutine'e aktarılabilir mi? Bu durumda fonksiyonun geri dönüş değeri türü coroutine_handle olmalı 
ama bu handle programın akışını başka bir coroutine'e değil coroutine'i durdurarak programın akışını onu çalıştıran koda geri döndürmelidir. Bu amaçla fonksiyonun return ifadesi

std::noop_coroutine_handle()
yapılır.

Bir koşula bağlı olarak çalıştırılan coroutine'i durdurmamak (suspend etmemek) için fonksiyonun geri dönüş değeri fonksiyonun parametre değişkenine aktarılan handle değeri yapılabilir.

await_resume()
--------------
Sonucu (veya hatayı) coroutine'e iletmekten sorumlu fonksiyondur.
await_suspend() tarafından başlatılan çalışma sırasında bir hata meydana gelmişse bu işlev yakalanan exception'ı
yeniden throw edebilir veya bir hata kodu döndürebilir. Tüm co_await ifadesinin değeri await_resume() fonksiyonunun döndürdüğü değerdir.

await_suspend() burada anahtar fonksiyondur. Bu fonksiyonun parametre değişkeni ve geri dönüş değeri aşağıdaki gibi olabilir:
await_suspend() fonksiyonunun parametresi şunlar olabilir:
Coroutine'in kendi handle türü : std::coroutine_handle Tüm coroutine handle'ları _için kullanılabilen temel bir tür: std::coroutine_handle (veya sadece std::coroutine_handle<>)
Bu durumda, promise nesnesine erişilemez. Burada parametre için auto specifier kullanılarak derleyicinin tür çıkarımı yapması sağlanabilir.

std::coroutine_handle<> türü
----------------------------
yerine başka bir coroutine'i devam ettirmek için.
Bu await_suspend() kullanımına simetrik aktarım (symmetric transfer) denir ve daha sonra ayrıntılı olarak ele alacağız.
Bu durumda, bir noop coroutine herhangi bir coroutine'i devam ettirmeme sinyali vermek için kullanılabilir (fonksiyonun false döndürmesi ile aynı şekilde).
Ek olarak, aşağıdakilere dikkat edilmelidir:

Üye fonksiyonlar, awaiter'ın değiştirilebilen bir veri elemanına sahip olduğu durumlar haricinde genellikle const'tır (örneğin, coroutine handle'ını
await_suspend fonksiyonu içinde saklayarak yeniden başlatma sırasında kullanılabilir hale getirmek gibi).
Üye fonksiyonlar genellikle noexcept'tir (final_suspend() fonksiyonu noexcept olduğu için zaten bu fonksiyon içinde kullanıma izin vermek için bu gereklidir).
Üye fonksiyonlar constexpr olabilir.
co_yield operatörünü kullanarak bir coroutine'in kendisini çağıran koda değer ya da değerler döndürmesini sağlayabiliriz:

co_yield C++20 standardı ile dile eklenen bir anahtar sözcük.

co_yield expr;
-------------
biçiminde kullanılıyor. Yukarıdaki ifade aşağıdaki derleyici tarafından aşağıdaki koda dönüştürülüyor:

co_await prom.yield_value(expr);
--------------------------------
Böylece promise nesnesinin co_yield operatörünün operandı olan ifadeyi kullanmasını sağlamış oluyoruz.

Bir coroutine'in sonunu blirtmek için co_return operatörü kullanılır. co_return C++20 standardı ile dile eklenen bir anahtar sözcüktür.
Bir coroutine'in tamamlandığını bildirmesi için üç yol vardır:

coroutine'in, son bir değer olarak expr ifadesinin değerini döndürmesi için
co_return expr;
deyimi kullanabilir.

Bu durumda, derleyici prom ilgili promise nesnesi olmak üzere, kodu

prom.return_value(e);
çağrısına dönüştürür.

coroutine'i son bir değer olmadan (değersiz olarak) sonlandırmak için
co_return;
deyimi kullananılabilir.

Bu durumda, derleyici prom promise nesnesi olmak üzere kodu

prom.return_void();
çağrısına dönüştürür.

coroutine'in yürütülmesiyle programın akışı fonksiyonun kodunun sonuna kadar gelir. Bu durumda, derleyici prom promise nesnesi olmak üzere koda
prom.return_void();
çağrısını ekler.
co_return hakkında dikkat edilmesi gereken önemli bir nokta var: co_return deyimi kullanılmadığı sürece return_void() fonksiyonunun tanımlanmamış olması bir sorun oluşturmaz. Ancak co_return deyimi kullanılmışsa ve promise sınıfının return_void ya da return_value fonksiyonları yok ise olmayan fonksiyon sentaks hatası oluşturur. Eğer programın akışı bir coroutine fonksiyonun sonuna kadar gelirse ve promise_type sınıfının return_void() fonksiyonu yok ise bu durum tanımsız davranıştır.

Bir coroutine'in tamamlanıp tamamlanmadığını öğrenmek için coroutine_handle nesnesini kullanarak std::coroutine_handle sınıfının done isimli fonksiyonunu çağırabiliriz:

bool coroutine handle<>::done();
Fonksiyonun true değer döndürmesi coroutin'in çalışmasını tamamladığını false değer döndürmesi ise coroutine'in henüz çalışmasını tamamlamadığını gösterir.

co_return ile ilgili dikkat edilmesi gereken diğer bir nokta daha var: hem promise_type::return_void() hem de promise_type::return_value() void fonksiyonlardır. Yani bir değer döndürmezler. Yani diğer fonksiyonlardan farklı olarak bu fonksiyonların geri dönüş değeri awaitable türlerinden değildir.
Peki bir coroutine'in sonunda ne yapılır? Derleyici coroutine durumunu güncellemeli ve coroutine'i son bir kez durdurmalı mıdır, böylece co_return yürüt sonra bile ana işlevdeki kod promise nesnesine erişebilir ve coroutine_handle'ı mantıklı bir şekilde kullanabilir mi? Yoksa bir coroutine'den dönüldüğünde coroutine_handle::destroy() fonksiyonuna yapılan örtülü bir çağrı gibi coroutine frame'i otomatik olarak yok mu etmelidir? Bu soru promise_type'ın final_suspend fonksiyonu ile çözülür. C++ standardı, bir coroutine'in fonksiyon gövdesinin aşağıdaki gibi sarmalandığını söyler (pseudo code)

{
	promise-type promise promise-constructor-arguments ;
	try {
		co_await promise.initial_suspend() ;
		function-body
	} catch ( ... ) {
	if (!initial-await-resume-called)
		throw ;
		promise.unhandled_exception() ;
	}
	final-suspend :
	co_await promise.final_suspend() ;
}
// "The coroutine state is destroyed when control flows // off the end of the coroutine"
Bir coroutine return ettiğinde, örtülü olarak çağrılan promise.final_suspend() işlevinin geri dönüş değeri co_await operatörünün operandı olur.
Eğer final_suspend işlevi gerçekten coroutine'i durdurursa, coroutine state son bir kez güncellenir ve geçerli kalır. coroutine dışındaki kod, 
coroutine handle'ın destroy() fonksiyonunu çağırarak coroutine nesnesini yok etmekten sorumlu olur. Eğer final_suspend coroutin'i_ 
askıya almaz ise, coroutine state otomatik olarak yok edilir.

Eğer coroutine durumuna bir daha dokunulmayacak ise (belki de coroutine co_return'den önce global bir değişkeni güncellediği ve/veya bir 
semaforu serbest bıraktığı içindir ve sizin için önemli olan tek şey budur), o zaman durumu son bir kez kaydetmek için gereksiz maliyete ve 
coroutine durumunu manuel olarak deallocate etmek için endişelenmeye gerek yoktur. Bu nedenle final_suspend() fonksiyonunun std::suspend_never döndürmesi sağlanabilir.


Öte yandan, bir coroutine return ettikten sonra coroutine handle'ına ya da promise nesnesine erişmeniz gerekiyorsa, 
final_suspend() işlevinin std::suspend_always (veya coroutine'i askıya alan başka bir awaitable nesne) döndürmesi gerekir.



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

