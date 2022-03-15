/*

Policy Based Design da kaldık.
Son yazılan örnek gösterildi tekrar.

NOT : TEMPLATE KODLARDA İSMİ NİTELEMEDEN KULLANIRSAK BU İSİM TABAN SINIFTA DOĞRUDAN
ARANMIYOR.TABAN SINIFTA ARANMASI İÇİN(DAHA ÖNCEDE YAZMIŞTIM)
1 - TABAN SINIFIN İSMİ İLE NİTELENEBİLİR
2 - THIS POİNTERINI KULLANMAK
3 - USING BİLDİRİMİ

POLICY_6

POLICY_7

OBSERVER
WEATHERDATA
Bir nesne var ve bu nesnede drum değişikliği olduğunda bazı başka sınıflar türündne nesnelerin
bu durumdan haberdar olması gerekiyor ve haberdar oldukları gibi kendi durumlarınıda
o nesnedeki durum değişikliğine göre ayarlamaları gerekiyor.

Mesela data değiştiği için termometre nesnesindeki gösterge değişti gibi.

Durum değişikliği olduğunda statei değişen veya bir event gerçekleştiğinde bu nesnenin
bu değişiklikle ilgili olan nesneleri bu durumdan haberdar etmesi.

Burada yüksek düzeyde bir coupling olmadan kod ekleyerek var olan kodları değiştirmeden
bunun sağlanması, amaç bu.

weatherdata örneğinde değişikliği sınıfların kendisi bildiriyor.
Yaygın olarak kullanılan update interfaceinin şöyle oluşturulması gerekiyor.Gözlemciler belirli değişiklikler için kayıt yaptıracaklar.
Kayıt interfaceinde hangi tür değişiklik olması durumunda update functionu çağrılacağı update funcının intetrfaceinde belirlenecek.
Mesela sınıfın hangi üye funcı çağrılacak gibi bir yapı olabilir.

Bunun dışında birden fazla subjectin gözlenmesi.

VISITOR VE DOUBLE DISPATCH
DOUBLE DISPATCH
Double dispatch bir nesnenin türüne değilde birden fazla nesnenin dinamik türüne göre seçilmesi.

VİSİTOR
İstediğimiz zaman yeni bir operasyon eklemek istiyoruz.

*/
