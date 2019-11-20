### Многокатегорийная абстрактная фабрика

На вход фабрике подаётся дерево наследований (интерфейс из задания)

Интерфейс работы:

получение фабрики 

`using MyFactoryHierarchy = FabricGenerator<3,5, Typelist<Chair, Table, Sofa>, Typelist< WoodenChair, WoodenTable, WoodenSofa>, Typelist< MetalChair, MetalTable, MetalSofa>, Typelist< MetalRichChair, MetalRichTable, MetalRichSofa>, Typelist< MetalPoorChair, MetalPoorTable, MetalPoorSofa> >`
`auto* MyFactory = new MyFactoryHierarchy::GetConcreteFactory<MetalRichChair>::result`

С помощью шаблонной функции Create можно создавать конкретные изделия, которые будут кастоваться сразу к нужному типу с помощью шаблона. 

*вместо auto можно было прописать для конкретной иерархии using AbstractFactory, но я считаю, что с auto проще и красивее*