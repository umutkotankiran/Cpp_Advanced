/*

TASARIM KALIPLARININ C ++ İLE İMPLEMENTASYONU
---------------------------------------------

TASARIM KALIPLARI/DESIGN PATTERNS
---------------------------------
Burada bahsedilen object oriented programmingte kullanılan tasarım kalıpları.
Bu konu baya bir geniş.Ayrı bir uzmanlık alanı bu. C++ programcısı olarak belli bir yere kadar
bilmeliyiz.Mimari tasarımcı olarak çalışılacaksa bilgiler artırılabilir.

Burada kullanılan terminoloji konusunda tam bir birliktelikte yok.
Oturmuş türkçe bir terminolojide yok.

Analiz bir problemi anlama, parçalara ayrılma süreci. Problem derken bizim uygulama geliştireceğimiz konu
anlamında. 

Dizayn problemin çözümünün kavramsal tanımı. İmplementasyon değil. Daha sonra bu dizayn implementasyona dönüştürülüyor.

Analizde ne yapmamız gerektiğini araştırıyoruz.Tasarımda onun nasıl yapmamız gerektiğini araştırıyoruz.

Mimari kelimesi sistemin daha üst düzeyde tasarımı demek. Tasarımın üzerinde yani.

Yazılım geliştirmede değişkenlik çok önemli. Yazılımlar doğası gereği sürekli değişim içindeler.
Değişim normal.Yeni talepler olabilir, programın başka bir donanıma taşınması, bug fixing, performans iyileştirmeleri...
Bir çok sebepten ötürü yazılım değişiyor.Kötü bir tasarım sözkonusu ise değiştirmek artık imkansız hale geliyor.

----------------------------------------------------------------------------------------------------------------------------------

PROBLEMLER
==========

Rigidity
--------
Katılık.
Eğer kötü bir tasarım varsa, yapılması gerekenler yapılmamışsa değiştirmek imkansız hale geliyor.
Bazı kodlarda değişiklik yapmaya cesaret edilemiyor. Katılık/Rigidity kodun değişime karşı direnç göstermesi.
Kötü tasarımın tipik sorunlarından biri, değişiklik yapmanın zor olması programın katı olması yani 
değişikliğe karşı direnç göstermesi.

Fragility
---------
Kırılganlık.
Değişiklik yaptıpımızda o değişikliğin başka yerlerde de etkileri oluşmasına deniyor.
Mesela bir değişiklik yaptık başka modüllerde problem çıktı. Çok kötü bir durum.
Ne zaman ne olacağının bir garantsi yok.Düzenleyince dahada kötü hale geliyor.
Refactoring konusu bilinmeli? Clionda bu feature olarak var.

Immobility
---------
Taşınmazlık.
Projenin belirli parçalarının başka yerde kullanılmasının zorlaşması. Reusability nin zorlaşması. 
Bağımlılıklar yüzünden başka yerde çalıştıramıyoruz kodu.Çalıştırmak için bağımlı olduğu kodlarıda almamız gerekiyor.
Alıp izole edip kullanamıyoruz. Sonuçta varolanı kullanmak yerine yeni bir kod yazılması gerekiyor.Yani kodu tekrar kullanmak
zor durumda oluyor.

Viscosity
---------
Akışkanlık.
Daha önceden oluşturduğumuz bir tasarım var vedeğişiklikleri o tasarıma uygun yapmak istiyoruz.
Baştan konulmuş ilkeleri uygulayarak tasarımın ana yapısını bozmadan, ilk tasarıma uygun şekilde yapmak istiyoruz.
Doğru bir çözüm uygulamak istiyoruz.Fakat öyle bir hale geliyorki tasarıma uygun iyi çözüm, tasarıma uymayan kötü yanlış 
mimariye uygun olmayan bir çözüme göre çok daha zor hale geliyor.Bu sefer HACK'ler başlıyor.Kestirme yollar.
Bu hacklerin nasıl çalıştığını biel programcılar anlamyır bazen çünkü doğrusunu yapmak çok daha fazla çabaya mal oluyor.
Bu durum tasarımın vizkozitesi yüksek demek.Yani vizkozitesi yüksek olunca doğru olmayan yöntemler daha kolay uygulanır oluyor.
Biz vizkozitesi düşük tasarım istiyoruz.

Needless Complexity
-------------------
Gereksiz karmaşıklık.
Tasarımın mimarı yapısına doğrudan fayda sağlamayan bir altyapı oluşuyor.
Beklenmeyen durumlara karşı hazır olmaya çalışmak kodu daha karmaşık hale getiriyor.
Yani mimaride fayda sağlamayan öğelerin bulunup ve giderek artması demek

Needless Repetition
-------------------
Gereksiz tekrar.
Aynı kodun sürekli tekrar etmesi.Burada copy paste eğilimi artmış.
Bu çok kötü bir durum. Bir değişiklik gerektiğinde gereksiz tekrarlar olduğundan her değişikliğin ayrı
bir yerde ayrı bir bağlamda yapılması gerekiyor.
Gerekli soyutlamaların yapılmamasından ötürü oluyor.

NOT!!!!
Over Engineering : Bu ihtiyaç olmamasına rağmen belirli eklemelerin yapılması.Needless Repetition ise 
ileride bir problem ile karşılaşma endişesinden dolayı yapılan eklemeler.

Opacity
-------
Anlaşılmazlık.
Kodları okumak ve anlamak zor. Kodlar niyeti ifade etmez oluyor ya refactoring yada yeni tasarım zorunlu hale geliyor.
Kodlar proje ilerledikçe dahada opak hale geliyor. Kodları anlamak için çaba sarfetmek gerekiyor.
Böyle yazılmış kodları değiştirmekten kaçınır hale geliyoruz artık.

=================================================================================================================================
=================================================================================================================================
=================================================================================================================================
=================================================================================================================================

TASARIMLA İLGİLİ ÇOK ÖNEMLİ 2 KAVRAMA GEÇİLECEK. ÇÜNKÜ DESIGN PATTERNS BU 2 KAVRAMLA İLGİLİ.
COHESION VE COUPLING. BUNLARDAN BAHSEDEBİLMEK İÇİN ÖNCE MODÜLARİTEDEN BAHSETMELİYİZ.

MODÜLARITE
----------
Yazılım programı tipik olarak modüler bir yapıda.Taskler tipik olarak modüllere bölünmüş durumda.
Modüllerinde kendi bileşenleri var ve biz bunları ayrı varlıklar olarak ele alıyoruz.

Modülarite olmadan büyük işlerin parçalara ayrılması bölünüp çözülmesi çok daha zor.Modülarite daha iyi abstraction sağlıyor,
hiyerarşik yapılanmayı oluşturuyor, tasarımı daha anlaşılır hale getiriyor, managementı kolaylaştırıyor,
implementasyonu kolaylaştırıyor, daha verimli bir geliştirme ortamı sağlanıyor, refactoring daha kolay oluyor.

Burada bir modülün kendi içindeki öğelerle ilgili nitelikleri var, birde modüllerin başka modüllerle ilişkilerinin kalitesi var.
Burada cohesion ev coupling terimleri devreye giriyor.

COHESION
--------
Bir modülün elemanlarının birbirine bağlılığının ne kadar güçlü ve ne kadar zayıf olduğunun bir ölçüsü.

Sınıfa verilen sorumluluk ne kadar birbiriyle ilgili ise cohesion o kadar yüksek demektir.
Bizim istediğimiz high cohesion.

Cohesionu yüksek olan sınıfların, bakım maliyeti düşüyor, karmaşıklığını azalıyor, reusability'si daha kolay.

Cohesionu düşükse nasıl olur? Mesela utility sınıf böyle.malloc calloc atoi rand exit abort qsort bsearch...
Birbiriyle çokta ilgili olmayan funclar burada. Ortak amaca yönelik odaklanma yok.Cohesive değil.
Onuda yapsın bunuda yapsın diye birşey yok.

Mesela üye funclar veri elemanlarını ne ölçüde kullanıyor ? 
Bir member func bir veri elemanını, diğeri ise diğer data member kullanıyor.
Ama bunun yerine 4-5 member func var ve hepsinin içinde doğrudan veya dolaylı veri elemanlarının tamamına yakını
kullanıyorsa bu daha cohesive bir yapıdır.


COHESION KATEGORILERI
---------------------

Coincidental Cohesion : Olabilecek en kötü durum.Bunlar birarada olsun diye biraraya getirmişler. 
Çok ilgilide değiller. Parçalar rasgele biraraya gelmiş. Birden fazla sorumluluk var.
Farklı işler yapan bileşenler var. Bu bizim istediğimiz cohesion değil.

Logical Cohesion : Sınıfın öğeleri input işlemlerini yapıyor.Okuma işlemleri de olabilir.
Mantıksal açıdan birbiriyle alakalı ama kod açısından tamamen farklı.Buda istenen değil.

ör:
class Classxyz{
	double CircleArea() = default;
	double RectangleArea() = default;
	//.. 
	// istenilen değil
};


Temporal Cohesion : Zamana bağlı birliktelik var. Modülün öğeleri runtime da benzer zaman diliminde kullanıldıkları
için biraraya getirilmişler. Değişiklik yapmak önemli. Birden fazla değişiklik gerektiriyor. Kötü

ör:
class Init{
	initPrinter
	initMonitor
	initDisk	
};


Procedural Cohesion : Diğerleri kadar kötü değil. Modülün elemanları taskın farklı aşamalarını içeren
functionları biraraya getirmiş. Burada eylemler arası bağlantı zayıf. Tekrar kullanmaya yatkınlığı düşük.

ör:
önce db ye bağlan
sonra sorgu yap
sonra display et. yine kötü ama diğerleri kadar kötü değil.


Comminucational/Informational Cohesion : Öğelerin biraraya gelmesi aynı data üzerinde işlem yapmaları.İşlemler birbirinden farklı
ama kullanılan veri ortak

Sequential Cohesion : Modülün elemanlarından birinin çıktısı diğerinin girdisini oluşturuyor.Bu iyi bir cohesion.
Dosyadan oku, bunları veri yapısında tut onun çıktısını başkasına ver gibi.Fabrika üretim hattı gibi.

Functional Cohesion : İdeal durum yani olabilecek en iyisi.Modülün elemanları aynı görevi yerine getirmek için
biraradalar ve burada hepsi gerekli.Diğerlerinde birini çıkartıp ayırabiliriz ama bunda elemanlardan biri olmasa iş yapılamayacak.


----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------

COUPLING
--------
Programı oluşturan modüllerin birbirleri arasındaki ilişkilerin kalitesini niteleyen bir ölçü.
intramodule değil(module içinde değil), inter module(modüller arası).

Couplingin yüksek olması bizim için kötü. Biz modellerin bağımsız olmasını isteriz.
Modüller arası bağlılık ne kadar varsa, coupling o kadar yüksektir. İstediğimiz düşük coupling.
Coupling 0 da olamaz. Moduller belirli seviye ilişkiye girecek ama önemli olan bu ilişkinin yoğunluğu.

İstediğimiz couplingin düşük cohesionun yüksek olması ama herzaman mümkün değil tabi.

Couplingin yüksek olması yazılımsal hataların en büyük kaynağı.Eğer coupling yüksekle bir modüldeki
değişiklik başka modüllerinde değiştirilmesine yol açıyor.
Fragility ile coupling arasında doğrudan bir ilişki var.Coupling ne kadar yüksekse fragility riskide o kadar artıyor.
Bu yüzden koduda hiç anlamıyor hale geliyoruz.

Content Level Coupling
----------------------
Olabilecek en kötü coupling.Zaten programlama dilleri bunu belli ölçüde engelliyor.
Bir sınıfın başka bir sınıfın veri elemanını değiştirdiğini düşünelim.Bir modül başka bir modülün implementasyonuna erişiyor.
Data hiding ilkesi ihlal edilmiş oluyor. Kötü bir durum.Yada başkabir modülün veri yapısını değiştirdiğini düşünelim.

Common Coupling
---------------
Birden fazla modüle aynı global dataya okuma ve yazma amaçlı erişiyorlar.
En tehlikeli şey global veride değişiklik. Fragility nin en tipik nedenlerinden biri.
Bu tamamen antipattern olarak görülüyor. Ortada bir data var ama sorumluluğu kimde belli değil.
Veriye kimin eriştiğini kontrol etmekte zor.

Control Coupling 
----------------
Bir modüle başka bir modülün akış biçimini belirleyebiliyor yani kontrol edebiliyor.
Bu her zaman kötü olmak zorunda değil. Zaten yapı buna dayanıyorsa ve birbirlerinni iç yapılarını bilmeleri
sözkonusu değilse ama akışını kontrol etme onun implementasyonuylada ilgiliyse bu çok çok kötü.
yani factoringe izin veriliyorsa, iç işleyişle ilgili temas yoksa o kadar kötü olmayabilir.

Data Structure Coupling
-----------------------
2 modül arasında bir veri yapısının kullanımı açısından bağlantı var.
Ör: Modülümüzde veri yapısı var. Bu veri yapısının belli bir kısmını normalde bu veri yapıs9ına erişimi olmayan modüle geçiyoruz.
Eğer diğer bileşen veri yapısının bu işi yapabilmek için veri yapısını nasıl manipüle edileceği bilgisine sahipse bu kötü.
Kötü ama çok sık uygulanıyor. C++ genellemelere uymuyor :D:D
Amaç efficienty ise o zaman bazı ilkelerden feragat edebiliyoruz.

Data Coupling
-------------
En ideal durum. 2 modül birbirine sadece data geçiyor. Örneğin parametreler yoluyla iletişim sağlıyorlar.
Couplingi azaltmanın temel yollarından biri functional design. Modüllerin sorumluluklarını functional olarak
sınırlandırmak. Geliştirme yapılırken birbiriyle alakası olmayan herşeyin ayrı ayrı geliştirilmesi.
Aynı çağrışımda olanlar birarada geliştirilsin.

Birarada geliştirilen öğeler yüksek cohesion içinde olmalı. 


----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------

NESNE YÖNELIMLI PROGRAMLADA KULLANILAN TERIMLER VAR
Kişiden kişiye farklı kullanılabilir.

Guideliness
Principals
Patterns
Styles
...

Bunlar soyutlama seviyeleriyle ilgili temel olarak.

Object Oriented Dizayn ile ilgili
En yukarıdaki kavramlar / Main principals / First Level
	abstraction, encapsulation, hierarchy, inheritance, aggregation, modularity ...

Onun altında ilgeler var / Seconds Level
	solid principal, grasp principles, law of demeter ....

Third Level
	Design patterns

Heuristics
	...

Guidelines
	...

----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------

2 AYRI ILKE GRUBUNDAN BAHSEDILECEK

SOLID
-----
[S]ingle Responsibility Principle (SRP)
[O]pen Closed Principle (OCP)
[L]iskov Substitution Principle(LSP)
[I]nterface Segreagation Principle(ISP)
[D]ependancy Inversion Principle(DIP)

*/
