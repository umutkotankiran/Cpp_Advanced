/*

ATOMİK TÜRLER
-------------
Atomik işlem, bölünemeyen bir işlem demek. Öyle bir işlemki birden fazla parçayla yapılıyor olsa dahi
o parçaların arasına başka threadlerden başka işlemler giremiyor demek.

Ör:
++x 

read 

modify

write işlemleri yapılarak yukarıdaki işlem gerçekleşiyor

Write tan önce başka bir thread x i okuyabilir. Bu işlemler atomik olarak yapılmazsa ve böyle
değişkenler birden fazla threadde paylaşılan değişkenler ise bir data race oluşturabilir ve bu da UB demektir.

Atomik işlem olması durumda bir garanti var.
Bu işlemin atomik olması demek, bir başka threadin ya bu işlem tamamalanmadan
bellekte x i görmesi ya da bu işlem tamamlandıktan sonra x in değişmiş değerini görmek
demek. Ara bir state te x i görmüyor. Bu garanti veriliyor.

Bu özelliğe sahip değişkenler/atomik değişkenler torn read tornw write denilen disk durumlarına sahip değiller.

C++ library atomik işlemlerin yapılabileceği sınıf türlerine sahipler.2 sınıf var aslında biri template.
Bu templatten oluşturulan explicit ve partial specializationlar var.

Yani biz int türden değişken kullanmak yerine atomik özelliğe sahip int i sarmalayan bir atomik değişken
kullandığımızda o değişken üzerinde yapılan bazı işlemler atomik olarak gerçekleşecek.

NOT!!!!
Tipik yanlış anlama
2 ayrı işlem atomikse, bu iki işlemin oluşturduğu bir ifadedeki işlemlerin toplamı atomik olmak zorunda değil.

++x + y++  --> x te y de atomik özelliğe sahip değişkenler olsa, bu işlemin tamamı atomik değil.
x++ bundan sonra x i kullansak, bu işlemin toplamının atomik olma zorunluluğu yok.Araya işlem girebilir.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

BIR DEĞIŞKENIN ATOMIK OLMASI NE DEMEK ? 
Bir değişken atomikse, ++ işlemide o atomik değişken üzerinde yapılabilecek atomik bir işlemse 50 tane
thread oluştursak, 50 threadde 50 tane task çalıştırsam, bu tasklerin hepsi x değişkenini 1 veya daha fazla artırsa
bunların artırmalarının hepsi gerçekleşecek anlamında.

İşlemlerin atomik olup olmaması işlemciye bağlı daha çok.Dolayısı ile işlemin atomik olması garantisi 
ile lock kullanma garantisi aynı şey değil. Belli bir tür için belli bir işlem atomik olabilir.
Fakat geri planda bu üretilen kodda cpu nun bir instructionu ile yapılabilir yada bunu implemente eden kod
arka planda bir kilit sistemi kullanıyor olabilir.

Yani atomik bir tür üstünde yapılan atomik bir işlem lock free olmak zorunda değil.İşlemin kendisini
implemente etmek için bir kilit sistemi kullanmıyor olabilir. Bu tamamen atomik olarak kullanılacak türün
hangi tipten veriyi sarmaladığı ile ve hedef işlemcinin ne olduğuyla ilgili. Standart library de bunu test
eden bir function var. Bunun ismi lock_free.

C++ IN TARTIŞMASIZ EN KARMAŞIK KONUSU BU KONU
Burada birinci kullanım seviyesi productionda çalışan programcıya ilişkin.Standart programcı.
İkinci seviyede ise low level interface var.Yani lock free denen programlama modeli.
Lock free, program runtimeda birden fazla thread kullanıyor olsada hiçbiri bloke olmayacak
her an bir thread ilerliyor olacak. Tüm threadler aynı anda da çalışabilir 1 threadde çalışabilir.
Bir sonraki aşama wait free programming, hiçbir thread beklemeyecek.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

ATOMIK TÜRLERI VE ATOMIK IŞLEMLERI NEDEN KULLANIYORUZ?
Atomik işlemler non divisible garantisi veriyor. Atomik işlemler tek alternatif değil.
Kilit sistemleri yani mutex kullanımı her zaman bir alternatif.Birden fazla tema var.
Temalardan biri verimlilik. Verimlilikte bedavaya gelmiyor. Yani mutex kullandığım heryerde 
büyük beceryile atomik türleri kullandım ve program daha verimli çalışıyor kesin diyemeyiz. Olabilir
olmayadabilir. Bu konuda verilen tavsiyeler var. 

Birinci Tavsiye, 
Eğer kilit sistemleri işimizi görüyorsa
herhangibir bottleneck yoksa veya herhangibir data race, deadlock, livelock problemleri riski yüksekse,
yani programın genel yapısı burada kullanılan mutexlerle ilgili bu garantiyi verme konusunda tereddütteyse
atomik tercihi kullanılabilir. Yani seçeneklerden biri doğrudan efficiency ile ilgili değil mutex kullanmanın
olası dezavantajlarından kaçınmak için. Bir nedende efficiency.

İkinci Tavsiye,
Atomik kullanıyoruz diye daha hızlı olacak dememeliyiz her zaman profile etmeliyiz.

Üçüncü Tavsiye
Platforma yönelik kestirimlerden kaynaklı atomikler kullanmayın deniyor.Bug oluşturma riskini çok artırıyor.
Yani belirli bier mimari için kod yazıyoruz, special instructionlar var. Bunların faydalarından faydalanmak için
atomik türlerin low level interfaceini kullanıyorum.

Low level derken, atomik türler üstünde yapılan işlemler belirli garantileri veriyorlar, dil bu garantileri
dokümante etmiş. Burada belirli garanti seviyeleri var. Sequencial consistency semantic gibi. Release acquire semantic gibi.
Yada release consume modeli gibi. Bunlar en yüksek seviyede interfacete görülen funcların 
default argümanlarla kullanılmış hali. Default argümanlarla kullanılmış halide her zaman sequencial
consistency. Daha fazla garantinin sağlanması programlamanın daha kolay, davranışın daha iyi kestirilebilir
olmasını sağlarken tabi ki işlemciyede bağlı ama ilave maliyet ekliyor.

Low level interface e geçmemizin sebebi aslında default argümanların kullanıldığı daha yüksek interface teki
verilen garantilerin belirli bir sistemde ilave maliyet getirmesi. Bir şey ilave maliyet getiriyorsa doğrudan
hemen ondan kaçınmak çok doğru olmayabilir.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

ATOMIC GITHUB NOTLARI !!!!

- Thread'ler arasında veriyi paylaşırken "data race" oluşmaması için genellikle bir senkronizasyon mekanizması gereklidir.
  (örneğin mutex kullanarak senkronizasyonu sağlamak).
- Veriye senkronize edilmiş erişim sağlamanın başka bir yolu da atomik türleri kullanmaktır.
- Bölünemeyecek (indivisible) bir işleme "atomik işlem" denir. Atomik bir işlem arasına başka bir işlem giremez.
  Örneğin arttırma işlemi atomik ise bu işleme tabi tutulan bir değişken diğer thread'ler tarafından ya işlemden önceki ya da 
  işlemden sonraki değeri ile görülebilir. İşlemcilerin çoğunda atomik bellek işlemleri özel makine komutları ile gerçekleştirilir.
- atomik bellek işlemleri "torn read" ya da "torn write" durumu oluşturmazlar.
- C++ standart kütüphanesi atomik bellek işlemleri için aşağıdaki atomik türleri sunmaktadır:
	std::atomic_flag
	std::atomic<>
- Bu türler donanımın sağladığı atomik operasyonlar için uniform bir arayüz sunarlar.


Atomik bir tür aşağıdaki garantileri sağlar:
- atomicity (atomiklik - bölünmezlik)
- ordering (bellek işlemlerinin sıralanması - önceliği sonralığı)
- visibility (bellek işlemlerinin sonuçlarının diğer thread'lerden görülmesi) Sıralı işlem garantisi 
  bellek işlemlerinin diğer thread'lerde ne şekilde görülebileceği garantisidir. 
  Atomik türler birbirinden bellek işlemlerinin sıralanması konusunda farklı garantiler sunarlar. 
  Varsayılan düzen "sequentially consistency"'dir. (daha sonra değinilecek)


atomic değişkenlerle yapılan işlemler atomik olma garantisini taşırlar. 
Yani bu işlemler tamamlanıncaya kadar bir başka thread aynı değişken üzerinde işlem yapamaz. 
Atomic bir değişkenin değerinin (atomik olarak) bir arttırıldığını (increment) düşünelim. 
Bu işlem aslında 3 farklı işleme karşılık gelir: oku - değiştir - yaz (read - modify - write). 
Eğer değişkenimizin atomik olma garantisi yok ise bu işlemlerin arasına başka thread'lerin 
işlemleri girebilir. Değişkenimiz atomik ise diğer thread'ler değişkenimizi ya değiştirilmeden 
önceki değeri ile ya da değiştirildikten sonraki değeriyle görebilirler, ara bir değerde göremezler. 
Bir başka deyişle atomik değişkenler için torn-read ya da torn-write olamaz.


std::atomic_flag
- std::atomic_flag türü sadece temel işlemler sunar.
- Bu türden bir değişken (flag) yalnızca iki durumda olabilir. Ya set edilmiş durumda (true) ya da 
  clear edilmiş durumda (yani false) 
- bu tür 2 operasyon sunar:
	-- test_and_set: state'i true yaparken set işleminden önceki değeri sorgular (get eder).
	-- clear: state' i false değere çekiyor.
- varsayılan kurucu işlev (default ctor) nesneyi belirlenmemiş bir değerle başlatır. 
  C++20 standartları ile bu durum değişti. Default ctor artık false değeri ile başlatıyor.
- Kopyalanamayan ve taşınamayan bir türdür.
- Derleyiciye bağlı olan ATOMIC_FLAG_INIT makrosu bu türden bir değişkene ilk değer vermek için kullanılır:


std::atomic_flag f = ATOMIC_FLAG_INIT;


- Bir kilit mekanizmasının kullanılmadığı garanti edilmektedir.
- Diğer atomik türlerde olan is_lock_free üye fonksiyonuna sahip değildir (zaten lock-free olmak zorunda).
- std::atomic_flag, spinlock mutex gibi daha yüksek seviyeli senkronizsayon mekanizmalarında araç olarak kullanılır.



std::atomic
std::atomic bir sınıf şablonudur. Bu sınıf şablonundan elde edilen türler atomik operasyonlar sunar.

TEMPLATE:
template <typename T>
struct atomic;

- Tam sayı türleri ve pointer türleri için bu sınıfın _"partial specialization"_ları var.
- T türünden bir nesne için atomik operasyonlar sağlıyor.
- Tüm temel türler (fundamental types) için standart kütüphane tarafından "full specialization" sağlanmış durumdda.
- Template argümanı olarak kullanılacak türün "trivially copyable" ve "bitwise equality comparable" olması gerekiyor.
- Kilit sistemi kullanılmama (lock-free) olma garantisi verilmiyor. Ancak platformların çoğunda temel türler olan 
  template argümanları söz konusu olduğunda bir kilit sistemi kullanılmadan (lock-free) gerçekleştiriliyor.
- Kopyalama ya da taşıma yoluyla nesne oluşturulamıyor. (not copy-constructible - not move constructible)
- Atama yapılabilir (assignable) bir tür. Ancak atama operatörü referans değil değer döndürüyor.
- Varsayılan bellek düzeni (memory order) std::memory_order_seq_cst.


-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

MEMORY MODEL
-----------
Bİzim programımızın(özellikle multithread çalışacakların) bellekteki read write işlemleriyle ve 
doğal olarak paylaşılan değişkenlerle ilişkisini formal olarak tanımlayan bir döküman.
Bir çok sorunun cevabı formal olarak memory model ile veriliyor.Asıl büyük devrim C++ a memory modelin eklenmesi.

Modern C++ öncesindeki durumu standartlar implementasyona göre değil C++ Abstract machine e göre belirlenen
soyut yapıya göre kuralları tanımlıyordu. Daha önceki C++ tanımı tamamen sanki tüm programlar single thread 
çalışacak gibi düzenlenmişti.

Multithread programlama library ile gerçekleştiriliyordu, dilin araçlarıyla değil.
Dilin standartları multithread programlara ilişkin herhangibir kural seti söylemiyordu.
Thread kavramı yoktu.

C++11 bir Memory Model getiriyor. DOlayısıyla birden fazla thread içeren bir programın runtimedaki
davranışını, bu memory model ve getirdiği kuralları doğrultusunda davranışını kestirebiliyoruz.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

GITHUB NOTLARI
--------------
MEMORY MODEL NEDIR?
- Bilgisayar programının belleğe erişiminde izin verilebilen semantik yapıyı tanımlar.
  Bellekten bir okuma yapıldığında hangi değerlerin okunması beklenebilir. Hangi durumlarda UB oluşabilir.
  Bunları kapsıyor. Bellek programa ve programda kullanılan threadlere nasıl görünüyor.Bu uygun cümle
  Threadlerin eş zamanlı çalışıyor olması değişkenleri eşzamanlı olarak aynı değerde göreceğimiz garantisi değil.
  Bu garanti ne zaman var ne zaman yok hangi ihtimaller sözkonusu? Bunları Memory Model tanımlıyor.
- Bellekten bir okuma yapıldığında hangi değer /değerler okunabilir? 
  (hangi değerlerin okunması beklenebilir)
- Hangi durumlarda tanımsız davranış oluşabilir?
- Memory model, multithread progranmlar için kritik olan bir bileşendir.
- Thread'ler bellek ve paylaşılan veril(ler) ile nasıl etkileşime giriyor? 
  Bellek programa ve programda kullanılan thread'lere nasıl görünüyor?
- Multi-thread programların yürütülmesine ilşkin kuralların belirlenebilmesi için 
  programlama dilinin bir memory model oluşturulması gerekiyor.


Thread'ler, programın çalışma zamanında paylaşılan verileri farklı değerlerde görebilir. Bir memory model oluşturmadan 
derleyici optimizasyonları ve donanım tarafında yapılan optimizasyonlar sorunlar oluşturabilir. 
thread'lerin paylaşılan değişkenlerde yapılan değişiklikleri anında gözlemleyebilmeleri verim (efficiency) açısından yüksek bir maliyet oluşturur.


KAYNAK KODUMUZDAKI IŞLEMLER
---------------------------
- Derleyici tarafından
- CPU tarafından;
- Bellek (caching) tarafından yeniden sıralanabilir


Memory model hangi durumlarda yeniden sıralamaya (reordering) izin verilip verilmediğini belirler.
Birden fazla thread hangi durumlarda paylaşılan değişkenlere erişebilir? 
Bir thread tarafından bir değişkene yapılan atama eş zamanlı çalışan thread'ler tarafından ne zaman görünür olacak? 
Programcılar multi thread programların nasıl çalışacağını (belirli garantiler altında) kestirebilmeliler. 
memory model buna yardımcı oluyor.

- C++11 öncesinde C++ dilinden formal bir bir memory model yoktu. 
  C++ soyut makinesi (abstract machine) single-thread olarak tasarlanmıştı. 
  java memory model 1995 yılında oluşturuldu. Yaygın kullanılan programlama dilleri içinde bu açıdan 
  java bir ilk. pthreads kütüphanesi ilk olarak 1995 yılında, bir memory modele dayanmadan geliştirildi. 
  Posix memory model oluşturmuyor. Tanımlar kesin (precise) değil. Yani, "program doğru mu" sorusunun formal bir cevabı yok.
  İlk olarak C++ dilinde java'nın memory modelinin kullanılması düşünüldü, ancak bunun uygun olmadığı görüldü (fazla kısıtılayıcı). 
  Java memory modeli belirli veri türlerinin atomik olmasını gerektiriyor. Java memory model C++ için çok pahalı. 
  Bir thread library oluşturabilemek için derleyici üstünde bazı kısıtlamaların olması gerekiyor.
  Bunun için de bir memory model'e ihtiyaç duyuluyor.

  
Bir programın çalışma zamanında nasıl davranacağını kestirebilmemiz için şunları bilebilmemiz gerekir:
- Programdaki (farklı thread'ler tarafından gerçekleştirilebilecek) işlemlerin (operasyonların) 
  hangi sırayla yürütüleceği (ordering)
- Programdaki bir işlemin/işlemlerin sonuçlarının (başka thread'ler tarafından yürütülebilecek) 
  diğer bir işlem/işlemler yapılmadan görülür olup olmadığı (visibility)

	x = 4;		b = y;
	y = x;		c = x;
	a = y;		z = y;

	x = 4 işlemi yapıldı ve bir side effecti var. eşzamanlı çalışan threadlerde c = x; ypılınca
	x in değişmiş değerini mi yoksa değişemiş değerini mi kullanacağız.
	Buna yönelik kurallar


Tek bir thread'in söz konusu olduğu programlarda yukarıdaki soruların cevabı sezgisel olarak verilebilir.
Birden fazla thread'in söz konusu olduğu programlarda ise bu durum çok daha karmaşıktır.



SEQUENCIAL CONSISTENCY
----------------------
Sırada tutarlılık. Programın çalışması sırasında farklı threadlerdeki farklı işlemler tek bir threadde sıralı
çalışıyormuş gibi olmak zorunda.Ama sırası aynı olmak zorunda değil. 45.33 resim var. 
sequencial consistency.png

Resimdeki örneğe benzer örnek verildi.

Derleyici burada reordering yapabilir. İşlemlerin sırasını değiştirdi ama bu değişiklik sonucu
yürütülen bu kodlar sırası değişmeden yürütüldüğünde oluşan sonuçların aynısı oluşacak.



HAPPENS-BEFORE ILIŞKISI !!!!!!!!!!!!!!!!!!!!
-----------------------
Bu standartların kulladığı bir terim. Happens before demek, zamansal olarak bir işlem bir işlemden
önce yapılıyor anlamına gelmiyor. Mesela fiziksel süre sözkonusu olduğunda bir işlem diğerinden
daha kısa sürede yapılıyordur ama bu happens before olmak zorunda değil. Hatta tersi bile olabilir.
A ve B arasında happens before ilişkisi var diyelim, ama fiziksel olarak baktığımızda A, B den daha
önce yapılmış olma zorunda da olmayabilir.


KRİTİK NOKTALAR !!!
---------------
A ve B iki operasyon olsun. Bu operasyonlar aynı thread'de gerçekleştirilen operasyonlar olabildiği gibi 
farklı thread'lerde gerçekleştirilen operasyonlar da olabilir. 

Eğer A operasyonunun etkileri B operasyonunu yürütecek thread'de 
B operasyonu yürütüldüğünde görülür durumda ise, bu A HAPPENS BEFORE B demektir.
Böyle bir garanti söz konusudur.

A HAPPENS BEFORE B demek, A operasyonu yapılmış, onun etkileri tamamlanmış, B yürütüldüğünde buna güvenerek
B operasyonları yapılabilir demek. Bu da şu demek, 2 nokta arasında A HAPPENS BEFORE B ilişkisi varsa
aynı yada farklı threadde, biz B nin yürütülmesi durumunda A daki oluşan işlemlerin side effectlerini
kullanabiliriz demek.

Happens-before ilişkisi "zamansal olarak daha önce gerçekleşme" ilişkisine göre çok daha güçlü bir durumdur !!!!!!!!!!
Yani happens-before in time. happens-before in time olması happens before ilişkisi olduğunu garanti etmiyor !!!!!
Happens before ilişkisinin olması için dilin tanımladığı bazı durumlar var.
Dil diyorki belirli koşullar olursa A ile B arasında happens before ilişkisi vardır.
Bir kısmı senkronizasyon ile ilgili diğerleri ilse biraz daha yan durumlar.

Eğer A operasyonu B operasyonuna göre zamansal açıdan daha önce gerçekleştiriliyor ise bu A'nın etkilerinin 
B'yi yürütecek thread'de B yürütülmeden önce görülür olma garantisi değildir. caching, store buffer vs gibi 
mekanizmalar operasyonun etkilerinin diğer thread'lerde görülmesini geciktirebilir.

Atomik değişkenler doğrudan bir happens before ilişkisi oluşturmuyor.Atomik değişkenlerin bazıları
kullanımları happens before ilişkisi oluşturuyor. Yani bir değişkenin atomik olması demek,
onun tanımladığı atomik işlemlerin atomik olarak yapılması demek. Ama doğrudan atomik kullandığımızda
bir happens before ilişkisi garanti değilmiyor.



SEQUENCED BEFORE ILIŞKISI !!!!!!!!!!!!!!!!!!!!!!!
-------------------------
Sequenced-before ilişkisi aynı thread'deki işlemlere ilişkindir.
Aynı threaddeki işlemler(özel durumlar hariç) programdaki sıraya göre yapılmak zorunda.

Sequenced-before ilişkisi geçişkendir (transitive) 
A sequenced before B doğru ise 
B sequenced before C doğru ise 
A sequenced before C doğrudur.

x = 1; 		// A  önce bu
y = 2; 		// B  sonra bu
z = x + 1;	// C  en sonda bu



Aşağıdaki kodu ele alalım:

y = a * x + b; // (y = ((a * x) + b);
Bu ifadede 3 işlem var: çarpma toplama ve atama. 
Burada
çarpma işlemi sequenced before toplama işlemi
toplama işlemi sequenced before atama işlemi
dolayısıyla
atama işlemi sequenced before toplama işlemi

------------------------------
------------------------------
Eğer bir thread için
A sequenced before B doğru ise
A happens before B doğrudur.
------------------------------
------------------------------

Bu şu anlama geliyor:
Sequenced-before ilişkisi aynı zamanda thread içindeki (intra-thread) happens before ilişkisine karşılık geliyor.
Happens-before ilişkisi zamana bağlı değildir, görünürlüğe (visibility) bağlıdır.

A happens before B ise, bu A'nın B'de önce yapılması anlamına gelmez (böyle bir garanti yoktur).
A, B'den önce yapılmış ise A happens-before B olmak değildir.
happens-before ilişkisi acquire-release semantiği ile gerçekleştirilebilir. (Daha sonra ele alacağım)
Happens-Before Does Not Imply Happening Before!


Aşağıdaki koda bakalım:

#include <iostream>

int is_ready{ 0 };
int value{0};

void producer()
{
	value = 42;                         // (1)
	is_ready = 1;                       // (2)
}

void consumer()
{
	if (is_ready)                       // (3) Burada okunan değerin 1 olduğunu düşünelim
		std::cout << value;             // (4)
}


Buradaki fonksiyonlar iki ayrı thread tarafından çalıştırılıyor olsun. 
Değişkenlere yapılan atamalar (stores) ve değişkenlerden yapılan okumalar (loads) atomik olsun. 
Programın çalışma zamanında consumer thread'inin (3) noktasına geldiğini ve is_ready değişkeninin okunan değerinin 
1 olduğunu düşünelim. Bu değer producer thread'inde (2) noktasında is_ready değişkenine atanan değer.
(2) (3)'ten önce olmuş olmalı. Ama bu (2) ve (3) arasında happens-before ilişkisi olduğu anlamına gelmez.
(2) ve (3) arasında happens-before ilişkisi olmadığı gibi (1) ile (4) arasında da happens-before ilişkisi yoktur. 
Bu yüzden (1) (4) arasındaki bellek işlemleri farklı şekilde sıralanabilir (reordering). 
Derleyici tarafından oluşturulan instruction'lar işlemci ya da bellek tarafından farklı şekilde sıralanabilir (instruction reordering / memory reordering). 
Consumer thread'i (4) noktasına geldiğinde ekrana 0 değeri yazılabilir.


- Inter-thread happens-before ilişkisi(farklı threadler arasındaki happens before ilişkisi)
  farklı thread'ler arasındaki happens-before ilişkisidir.
- A ve B farklı threadler'deki operasyonlar olsun. 
  Eğer A ve B arasında inter-thread-happens_before ilişkisi varsa, A ve B arasında happens-before ilişkisi vardır.
  Yani A'daki operasyonların sonucu B'de görülebilir olmak zorundadır.
- Inter-thread happens-before ilşkisi geçişkendir (transitive)
  A ve B arasında inter-thread happens before ilişkisi varsa,
  B ile C arasında inter-thread happens before ilişkisi varsa,
  A ile C arasında da inter-thread happens before ilişkisi olmak zorunda.
- Inter-thread-happens-before ilişkisinin oluşması için (dil tarafından tanımlanan) bir senkronizasyonun söz konusu olması gerekir.
- A ve B arasında happens-before ilişkisi olsun. Bu durumda
  -- A ve B aynı thread'de olabilir ve aralarında sequenced-before ilişkisi vardır.
  -- A ve B aynı farklı thread'lerde olabilir ve aralarında intra-thread-happens-before ilişkisi vardır.

GITHUB NOTLARI BURADA BITTI


-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------


HAPPENS BEFORE İLİŞKİSİNİN OLMASI İÇİN HANGİ SENARYOLAR SÖZKONUSU
-----------------------------------------------------------------

Synchronizes-With Relationship
------------------------------
Bu ilişki varsa farklı 2 thread arasında, inter-thread happens-before ilişkisini garanti 
altına alabiliyor. Yani inter-thread relationship sadece syncronizes with ile sağlanmak
zorunda değil ama birçok durumda happens before ilişkisini sağlayan synchronizes with ilişkisi.

Synchronizes with ilişkisi aynı zaanda happens before u garanti ediyor.

-------------------------------------------------------------------------------------------------------------------

1) Thread Creation 
------------------
Thread sınıfı türünden bir nesne oluşturup Constructruna workload verirsek
eğer ctorunun çalışması tamamlanmışsa workload olarak çalışan functionun çağrısıyla
bir synchronizasyon yaratmış oluyoruz.


ÖR:

#include <thread>
#include <cassert>

int shared = 0; // Bu değişkenin atomik olmasıda gerekmiyor

void func
{
	// B
	// bu kısımda sharedin 1 olması garanti altında
	assert(shared==1); // buraha hata olmayacak. Bu garanti altında
}

int main()
{
	shared = 1;
	/// A
	std::thread t{func};
	
}

-------------------------------------------------------------------------------------------------------------------

2) Thread join
--------------
Thread funcının çalışmasından sonra join funcının çalışmasyla beraber bir senkronizasyon
oluşturmuş oluruz.

#include <thread>
#include <cassert>

int shared = 0;

void func
{
	assert(shared==1); 
	++shared;
}

int main()
{
	shared = 1;
	/// A
	std::thread t{func};
	t.join();

	// Bu noktada shared değişkeninin 2 olması garanti altında
	assert(shared == 1); // hata vermez
}

-------------------------------------------------------------------------------------------------------------------

3) Mutex Unlock/Lock
--------------------
Unlock ile lock arasında bir senkronizasyon ilişkisi var ve bu da bir happens before
ilişkisi doğurur.

std::mutex m;
int x = 0;
int y = 0;

     A				  B
thread 1 code               thread 2 code
--------------		   --------------
m.lock();			   m.lock();
x = 1;		  |------------->  y = x;
m.unlock)---------|		   m.unlock)
	   Synchronizes with
	

kiliti thread 1 alsın, x = 1; yapılacak ve sonrasında thread unlock edildiğinde,
unlock ile y = x noktası arasında synchronizes with ilişkisi var. 
Yani B threadi x değerini 1 olarak görmek zorunda.
Yani A happens before B olmak zorunda.


-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

ATOMIC / ATOMİK
---------------
Header file atomic.h

Genel olarak C++ ta önce sınıfa member functionlar koyuyorlar.
Sonra bunlara global function alternatifleride veriyorlar. remove erase idiyomunda olduğu gibi.

Atomik türlerin birincil özelliği şu,
atomic isimli bir sınıf şablonu var. Bunun partial specializationlarını yada explicit specializationlarını kulanıyoruz.
Bazı durumlarda primary template ten user defined type lar için kendi atomic sınıflarımızı oluşturuyoruz.
Belirli koşullaır sağlayan custom typelar için primary template i, template argümanı olarak kendi sınıfımızı
argüman olarak veriyoruz.

Bir int i atomik olarak kullanmak için atomik sınıfının int açılımını
Bir unsigned longu atomik olarak kullanmak için atomik sınıfının unsigned long açılımını kullanıyoruz
Bunlar specializationlar. 

Diğer taraftan class data isimli sınıf var. Template argümanı koşullarını sağlıyor.
Bunun içinde trivialy copy able olmalı, bitwise comparable olmalı.
o zaman atomic in data açılımını kendi atomik türümüz için oluşturabiliriz.

Bir atomik tür haricinde hiçbir atomik açılımınun lock free olma garantisi yok.
garantisi olmaması başka lock free olmaması başka.

#include <iostream>
#include <atomic>

int main()
{
	using namespace std;

	std::cout.setf(std::ios::boolalpha);
	std::atomic<int> x; 

	std::cout << x.is_lock_free() << '\n'; // true ama lock free olmak zorunda değil.
}

-------------------------------------------------------------------------------------------------------------------

ATOMIC FLAG
-----------
Bu öyle bir sınıfki minimal olarak heryerde lock free olma garantisinde. Ayrı bir interface e sahip.

Her platformda lock free olmak zorunda ama atomik flag lock free olması implementasyon tarafında
lock mekanizması mutex mekanizması kullanılmıyor anlamında. Yani işlemler gerçekten bir mutex kullanmadan
doğrudan instructionlar yoluyla atomik.


int main()
{
	using namespace std;

	std::cout.setf(std::ios::boolalpha);
	std::atomic<long long> x;

	std::cout << x.is_lock_free() << '\n'; // true ama lock free olmak zorunda değil.
}

-------------------------------------------------------------------------------------------------------------------

BUNU COMPILE TIMEDA ÖĞRENME ŞANSI VAR.
Neden bu compile timeda öğrenilmiyor ? Bazı türler alignmeneta bağlı olarak sometimes lock free
olabiliyor bazı durumlarda da olmayabiliyor. Compile timeda is always lock free isimli constexpr static
veri elemanı var. always lock free isimli static veri elemanı true değerdeyse bu herzaman lock free demek.


#include <iostream>
#include <atomic>

int main()
{
	// Bu kodu visual studio implemente etmemiş olabilir, wandboxta test ettik. !!!!
	constexpr auto b = std::atomic<long long int>::is_always_lock_free;

	std::cout << x << '\n'; // long long türü always lock free. Eğer lock free değilse yada sometimes lock free ise false değeri
							// Aksi halde değeri true
}

-------------------------------------------------------------------------------------------------------------------

ÖR:

#include <iostream>
#include <atomic>

int main()
{
	std:atomic_flag f;  // Çok sınırlı interface i var. C++20 olmadan önce 2 funcı vardı.
						// C++20 ile genişlettiler.
}

-------------------------------------------------------------------------------------------------------------------

ÖR:

#include <atomic>
#include <iostream>

int main()
{
	using namespace std;

	cout << boolalpha;

	// atomic_flag flag_x{ false }; //gecersiz
	
	// atomic_flag flag_y{ true };  //gecersiz
	
	atomic_flag flag_z; //default init ile C++ 17'de belirsiz deger C++20'de false değeri
	
	cout << "flag_z = " << flag_z.test() << "\n"; //C++20. Flagin değerini get eder.
	
	atomic_flag flag = ATOMIC_FLAG_INIT;  //gecerli. C++20 değilse bu şekilde initialize edilmek zorundaydık
											// ATOMIC FLAG INIG değeri implementation defined.
											// Burada atomic flag değişkenimiz false değerine geliyor !!!!!
	
	cout << "flag = " << flag.test() << "\n"; //C++20.Flagin değerini get eder.
	
	auto b = flag.test_and_set();  // flagi true değere çekip eski değerini get ediyor
	
	cout << "b = " << b << "\n";
	
	cout << "flag = " << flag.test() << "\n";
	
	flag.clear(); // atomic flagin değerini false yapar
	
	cout << "flag = " << flag.test() << "\n";
	
	b = flag.test_and_set(); 
	
	cout << "b = " << b << "\n";
	
	cout << "flag = " << flag.test() << "\n";
}

-------------------------------------------------------------------------------------------------------------------

ÖR: Açıklamalardan değilde main den itibaren ne yapıldığına bak. Daha kolay anlaşılıyor.

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <iostream>

class SpinLockMutex {
public:
	SpinLockMutex()
	{
		m_f.clear();
	}

	void lock()
	{
		while (m_f.test_and_set()) // lock funcını return etmesi için bu funcın false dönmesi gerekir
		{			   // set edilen değerde true olur
					   // bu funcı çağırdığımızda zaten false dönerse demekki eski değeri false
					   // set edip döngüden çıkacak.
					   // mutexi başkası edinmişse o zaman zaten set edilmiş durumda
					   // set edilmiş durumda olduğundan bu func çağrıldığında true dönecek
					   // ne zaman mutex bırakıldığında atomic flag false ve olacak bu döngüden
					   // çıkacak.


			; //null statement      
		}
	}

	void unlock() // atomic flagi false değerine çekecek
	{
		m_f.clear();
	}

 private:
	std::atomic_flag m_f; // default construct ettiğimde bu false değerini alacak
};

SpinLockMutex mtx;
unsigned long long counter{};

void func()
{
	for (int i{ 0 }; i < 100'000; ++i) {
		mtx.lock();
		++counter;
		mtx.unlock();
	}
}

int main()
{
	std::vector<std::thread> tvec;

	for (int i = 0; i < 10; ++i) {
		tvec.emplace_back(func);
	}

	for (auto &th : tvec) {
		th.join();
	}

	std::cout << "counter = " << counter << "\n"; // 1'000'000 değerini verdi.
}

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

AŞAĞIDAKILER KARIŞTIRILMAMALI

atomic_flag başka bir tür ama atomic<bool> ayrı bir tür.
Ama atomic in diğer tamsayı türleri açılımı aynı interface e sahip.
Reel sayı açılımlarından belirli operasyon yok.
Bool açılımında artırma eksiltme yok
Birde pointer açılımları var.

ÖR:

#include <atomic>
#include <iostream>

int main()
{
	using namespace std;

	cout << boolalpha;
	atomic<bool> flag_1;
	atomic<bool> flag_2; //indetermined value before before C++20. false value since C++20
			     // atomik türlerin tamsayı ve bool açılımlarını default construct
			     // ettiğimizde C++20 öncesi indetermined value idi. C++20 ile
			     // value initialize edilmesi söz konusu.

	// atomic flag1 = false; // CTAD ile geçerli.

	cout << flag_1 << '\n'; // burada operator T gibi bir func var. tür dönüşt. operator func.
				// değeri doğrudan get ediyor. Bunun yerine load funcıda çağrılabilir.
				// fark şu, isimlendirilmiş funclar memory order parametresi alıyor
				// bu atomik işlemin hangi güvenceleri verdiğini belirliyor
				// Eğer bir func operator funcı olduğu için ilave argüman alamıyor
				// bu durumda consistency model doğrudan sequencial consistency
				
	cout << flag_2 << '\n';
	 
	// ------------------------------------------------------------------------------------------------------

	///atomic<bool> flag_3{flag_2}; //gecersiz
	//flag_1 = flag_2; //geçersiz

	// ------------------------------------------------------------------------------------------------------

	flag_1 = true; 
	flag_2 = false;
	flag_1.store(false); // store funcına değeri geçiyoruz.false yada true
	flag_2.store(true);

	// ------------------------------------------------------------------------------------------------------

	cout << "flag_1 = " << flag_1 << '\n';  // operator T den ötürü yine yazdırabiliyoruz.
	cout << "flag_2 = " << flag_2 << '\n';  // operator T

	// ------------------------------------------------------------------------------------------------------

	auto b = flag_1.exchange(true); // Verdiğimiz değerle set ediyor.Eski değeri get ediyor.

	cout << "b = " << b << '\n';
	cout << "flag_1 = " << flag_1 << '\n';  // operator T
	
	// ------------------------------------------------------------------------------------------------------

	cout << "flag_1.load() = " << flag_1.load() << "\n"; // load un return değeri atomik değişkenin değeri
	cout << "flag_2.load() = " << flag_2.load() << "\n";
}


a.store(true) --> değer veriyoruz
a.store(load) --> memory order argümanı geçebiliriz
a.exchange(true) -->  değeri set edip eski değeri return değeri ile alıyoruz
a.operator T() -->  değeri elde ediyoruz
a.load() -->  değeri elde ediyoruz

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

CAS
---
2 tane funcımız var.Aralarındaki fark 200 satır sonra açıklandı
compare_exchange_strong
compare_exchange_weak

compare_exchange_strong
1. parametresi T&

atomic<int> a;
bool result = a.compare_exchange_strong(expected, desired);
excepted int türden
desired 2. parametre. Olması gereken değer.

- Bu function çağrılmadan önce a nın değeri expected ise, a nın değerini değiştirip desired yapıyor.
  Bu durumda function true değer döndürüyor(değer değiştirildiyse eğer).
- Eğer a nın değeri expected değilse(yani 1. parametreye geçilen değer değilse) false dönüyor ve
  expected değerini a nın değeri yapıyor.

   
expected = 4, a = 7; // olsun
bool result = a.compare_exchange_strong(expected, 50);
a(7) nın değeri 4 olmadığı için functionun return değeri false olacak, expecteda ise a nın değeri olan 7 yi yazıyor.

expected = 4, a = 4; // olsun
bool result = a.compare_exchange_strong(expected, 50)
a nın değeri expected ile aynı ikiside 4. a nın değerini desired yapıyor ve true dönüyor.

Aşağıda 1 örnek daha var.

-------------------------------------------------------------------------------------------------------------------

ÖR:

#include <atomic>
#include <iostream>

int main()
{
	using namespace std;

	cout << boolalpha;
	atomic<int> a;

	cout << "a = " << a << "\n"; // c++20 ile a = 0
	cout << "a.load() = " << a.load() << "\n";

	a.store(10); // a ya 10 değeri store edildi
	cout << "a = " << a << "\n"; // 10

	int expected = 20; // expected 20 yapıldı
	cout << "expected = " << expected << "\n"; //20

	bool result = a.compare_exchange_strong(expected, 50); // a ile expected farklı değerlerde
								// functionun return değeri false olacak
								// a değişkeni set edilmeyecek.
								// result false olacak yani return değeri olan bool
								// expected ise a nın değeri olan 10 olacak

	// a has not the expected value and will not be set
	cout << "a = " << a << "\n"; // 10
	
	//result will be false
	cout << "result = " << result << "\n"; // false

	//expected will be set to the value of a
	cout << "expected = " << expected << "\n"; // 10. expected a nın değerine set ediliyor.
}


-------------------------------------------------------------------------------------------------------------------

ÖR:

#include <atomic>
#include <iostream>

int main()
{
	using namespace std;

	cout << boolalpha;
	atomic<int> a;

	cout << "a = " << a << "\n";
	cout << "a.load() = " << a.load() << "\n";

	a.store(10); 
	cout << "a = " << a << "\n";

	int expected = 10; 
	cout << "expected = " << expected << "\n";

	bool result = a.compare_exchange_strong(expected, 50); // Artık expected ile a aynı değerde. 
								// true return edecek
								// a nın değerini 50 yaptı
								//expecteda dokunmadı bu sefer.

	// a has not the expected value and will not be set
	cout << "a = " << a << "\n"; // 10
	
	//result will be false
	cout << "result = " << result << "\n"; 

	//expected will be set to the value of a
	cout << "expected = " << expected << "\n";
}

-------------------------------------------------------------------------------------------------------------------

ÖNEMLİ ÖRNEK

#include <atomic>
#include <iostream>

using namespace std;

int main()
{
	atomic<int> a = 10;

	// a *= 50; işlemini yapmak istiyorum ama *= işlemi atomik değil
	// sınıfında operator*= funcı yok
	// Bunu yapmak için a = a * 50; şeklinde yapmamız gerekir
	// ++x ile x = x+1; tamsayı olsaydı şey derdik ama atomik değişkenlerde böyle değil
	// ++a yazınca sınıfın operator++ funcı çağrılıyor ve bu işlemi atomik olarak yapıyor
	// a = a + 1; yapınca iki işlem yapıyor. a = a.load() + 1; demek.bir load birde atama işlemi var.

	// ----------------------------------------------------------------------------------------------

	//a yı 5 katına çıkarmak istiyoruz.

	atomic<int> a = 10;

	int temp = a.load();

 	a.store(temp * 50); // Buraya kadar olan kodda sorun yok ama burada problem çıkabilir.
			    // Bu diğer threadler tarafından da modifiye ediliyor.
			    // Bu satıra gelmeden önce başka bir thread a nın değerini 10 iken 100 yaptı diyelim
			    // Bizde bu durumda 10 * 50 değilde 100 * 50 yapmış oluruz.

	// Burada bir araca ihtiyaç var. Bu işlemi yaparken başka threadler modifiye etse dahi
	// modifiye edilmiş değeri kullanmam gerekiyor.(istediğimiz değeri)

	// ----------------------------------------------------------------------------------------------

	//Burada idiyom var.

	atomic<int> a = 10;

	int temp = a.load();
	
	
	// diyelimki a nın değerini başka thread değiştirdi, a = 200 yaptı diyelim
	while(!a.compare_exchange_weak(temp,temp*50))
		;
	// ilk turda a = 200, temp = 10. değerler aynı değil.false döndü. !false ile döngü devam etti.
	// bu durumda expected yani temp değerinide 200 yaptı.
	// 2. turda a = 200 ve temp(expected aslında) = 200, değerler aynı, bu durumda temp * 50 = 10000 değeri a ya store edildi
					
}

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

FUNCTIONLARA BAKALIM
--------------------
Operator funcları doğrudan atomik olarak desteklenen işlemler için var.

#include <atomic>
#include <iostream>

using namespace std;

int main()
{
	atomic<int> a = 10;

	//Aşağıdaki funclar var
	++a;
	a++;
	a--;
	--a;
	a = 5; // başka bir atomic atanamıyor tabi.
	a.load();
	a += 5;
	a -= 10;
	a &= 4;
	a ^= 4;
	a |= 4;
	
	// a *= 4;  BU YOK !!!!!!!!!!!
	
	auto result = a.exchange(450);

	//----------------------------------------------------------------------------------------

	COMPARE_EXCHANGE_WEAK İLE COMPARE_EXCHANGE_STRONG FARKI

	a.compare_exchange_strong ->  a nın değeri expected ise set garantisini veriyor
	a.compare_exchange_weak ->  Spurious olarak eşit olsada yani a nın değeri expected olsada
				    a nın değerini set etmeyip yine false döndürebiliyor. 
			  	    Compare exchange weak i doğrudan çağırırsak başırılı olup olmadığını
				    test etmemiz gerekiyor.

	Bizim örneğimizde compare_exchange_weak kullandık ama zaten her turda kontrol ediyoruz.
	Bizim örnekte sıkıntılı bir durum yok. Döngüsel yapılarda compare_exchange_weak kullnılabilir
	Döngüsel olmayan yapılarda compare_exchange_strong kullanılabilir.Kontrol etmeye gerek kalmıyor
	garanti veriyor çünkü. compare_exchange_weak kullansaydık değerini kontrol etmek gerekirdi.

	compare_exchange_weak bazı sistemlerde compare_exchange_stronga göre daha düşük maliyetli olabilir.
	döngüsel yapılarda weak kullanmak daha avantajlı oluyor.
}

-------------------------------------------------------------------------------------------------------------------

ÖR

#include <atomic>
#include <iostream>

int main()
{
	std::atomic<int> x; // C++20 ile 0

	int val = ++x; //atomic<T>::operator++()
	std::cout << "val = " << val << "\n";    1
	std::cout << "x = " << x << "\n";    1

	val = x++; //atomic<T>::operator++(int)   
	std::cout << "val = " << val << "\n";   1
	std::cout << "x = " << x << "\n";    2
}

Hatırlatma
++x ile x = x + 1 aynı anlamda değil.

-------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------

OPERATOR FUNCLARININ HEPSI MEMORY ARGÜMANI OLARAK SEQUENCIAL CONSISTENCY ALIYOR

#include <atomic>
#include <iostream>

int main()
{
	std::atomic<int> x;

	int val = ++x; // atomic<t>::operator++()

	x.fetch_add(1, std::); // bunun operator += ten tek farkı memory order parametresi alması
							// doğrudan operator overloading olanlar memory order parametresi almıyorlar
							// memory order parametresi almaması demek default olarak olarak sequencial
							// consistency modelini temsil eden memory order parametresini alıyor demek.

}

-------------------------------------------------------------------------------------------------------------------

ÖR:

#include <atomic>
#include <concepts>

template <std::integral T>
void atomic_inc(std::atomic<T> &x)
{
	T val{x};
	while(!x.compare_exchange_weak(val,val+1))
	{
	}
}

Burada c++17 için olan operasyonların ekran görüntüsünü aldım.
20de çok daha geniş. 

-------------------------------------------------------------------------------------------------------------------

ÖR:
Hoca yazmış bu sınıfı

NOT : Normalde atama operator funclarının return değerleri referans türüdür.
Atomik türlerde ise hiçbir zaman atama operator funclarının yada diğer funclarının
return değeri referans değil, doğrudan value.

#include <atomic>
#include <thread>
#include <iostream>

class AtomicCounter {
public:
	AtomicCounter() : m_c(0) {}
	AtomicCounter(int val) : m_c{val}{}
	int operator++() { return ++m_c; }
	int operator++(int) { return m_c++; }
	int operator--() { return --m_c; }
	int operator--(int) { return m_c--; }
	int get() const { return m_c.load(); }
	operator int()const { return m_c.load(); }
private:
	std::atomic<int> m_c;
};

AtomicCounter cnt;

void foo() // testi burada yapıyoruz.Değişkeni 1 milyon kere artırıyoruz
{
	for (int i = 0; i < 1'000'000; ++i) {
		++cnt;
	}
}

int main()
{
	std::thread ta[10];

	for (auto& th : ta)
		th = std::thread{ foo };

	for (auto& th : ta)
		th.join();

	std::cout << "cnt = " << cnt.get() << '\n';
	std::cout << "cnt = " << cnt << '\n'; 
}

-------------------------------------------------------------------------------------------------------------------

Pointerda bir atomik tür olabilir. User defined türde atomik olabilir.

int main()
{
	using namespace std;

	int a[] = {1,3,5,7,9,11};
	// int *p{a}; // Burada pointer kullanacağız diyelim ama atomik olmasını istiyoruz 
					// herhangibir sebepten ötürü

	atomic<int *> ax{a}; // bu şekilde kulanılır.

	std::cout << *ax.load(); // pointer gibi kullandık
	std::cout << *ax; // yine pointer gibi kullandık

	++ax;
	std::cout << *ax;

	ax += 2;
	
}

-------------------------------------------------------------------------------------------------------------------

Standartların verdiği örnek
Buna bak.

#include <atomic>
#include <iostream>
#include <thread>
#include <cassert>

std::atomic_bool x_flag, y_flag; //false değerlerde
std::atomic<int> ival; // 0 değerinde


void set_x()
{
	x_flag.store(true);
}

void set_y()
{
	y_flag.store(true);
}

void read_x_then_y()
{
	while (!x_flag.load())
		;

	if (y_flag.load())
		++ival;
}

void read_y_then_x()
{
	while (!y_flag.load())
		;

	if (x_flag.load())
		++ival;
}

//test kodu
void func()
{
	x_flag = false;
	y_flag = false;
	ival = 0;

	std::thread t1{ set_x };
	std::thread t2{ set_y };
	std::thread t3{ read_x_then_y};
	std::thread t4{ read_y_then_x};
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	assert(ival != 0);
}

int main()
{
	for (int i = 0; i < 10000; ++i) {
		func();
	}
}

*/
