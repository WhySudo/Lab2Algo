#include "gtest/gtest.h"
#include "../Array.h"



class ArrayTests : public testing::Test {
protected:
    template<typename T>
    void ConstructTest() {
        Array<T> a;
        EXPECT_EQ(0, a.size());
    }

    template<typename T>
    void FillTest(T *targetItems, int itemsCount) {
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {
            a.insert(targetItems[i]);
        }
        for (int i = 0; i < itemsCount; i++) {
            EXPECT_EQ(a[i],targetItems[i]);
        }
        EXPECT_EQ(itemsCount, a.size());
    }
    template<typename T>
    void InsertRemoveTest(T *targetItems, int itemsCount, int removeAmount) {
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {
            a.insert(targetItems[i]);
        }
        for (int i = 0; i < removeAmount; i++) {
            a.remove(0);
        }
        for (int i = 0; i < itemsCount-removeAmount; i++){
            EXPECT_EQ(a[i],targetItems[i+removeAmount]);
        }
        EXPECT_EQ(itemsCount-removeAmount, a.size());
    }
    template<typename T>
    void DirectIteratorTest(T *targetItems, int itemsCount){
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {
            a.insert(targetItems[i]);
        }
        typename Array<T>::Iterator iter = a.iterator();
        for (int i = 0; iter.hasNext()&&i < a.size(); iter.next(), i++){
            EXPECT_EQ(iter.get(), targetItems[i]);
        }
        EXPECT_FALSE(iter.hasNext());
    }
    template<typename T>
    void ReverseIteratorTest(T *targetItems, int itemsCount){
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {
            a.insert(0, targetItems[i]);
        }
        typename Array<T>::Iterator iter = a.reverseIterator();
        for (int i = 0; iter.hasNext()&&i < a.size(); iter.next(), i++){
            EXPECT_EQ(iter.get(), targetItems[i]);
        }
        EXPECT_FALSE(iter.hasNext());
    }
    template<typename T>
    void SelfAssigmentTest(T *targetItems, int itemsCount){
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {
            a.insert(targetItems[i]);
        }
        a = a;
        EXPECT_EQ(a.size(), itemsCount);
//        Array<T> b;
    }
    template<typename T>
    void AssigmentTest(T *targetItems, int itemsCount){
        Array<T> a; // normal
        for (int i = 0; i < itemsCount; i++) {
            a.insert(targetItems[i]);
        }
        Array<T> b(a); // copied by value
        Array<T> c; //assigned by value
        c = a;
        Array<T> d(a); // for future move
        Array<T> e(a); // for future move
        Array<T> f(std::move(d));
        Array<T> h =std::move(e);
        EXPECT_EQ(b.size(), itemsCount);
        EXPECT_EQ(c.size(), itemsCount);
        for (int i = 0; i < itemsCount; i++) {
            EXPECT_EQ(b[i], c[i]);
            EXPECT_EQ(b[i], a[i]);
            EXPECT_EQ(f[i], h[i]);
        }
    }
    template<typename T>
    void InsertPointTest(T *targetItems, int itemsCount, int insertPoint, T extraValue) {
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {

            a.insert(targetItems[i]);
        }
        a.insert(insertPoint, extraValue);
        typename Array<T>::Iterator iter = a.iterator();
        for (int i = 0; iter.hasNext()&&i < a.size(); iter.next(), i++){
            if(i == insertPoint){
                EXPECT_EQ(iter.get(), extraValue);
                continue;
            }
            int index = i > insertPoint ? i - 1: i;
            EXPECT_EQ(iter.get(), targetItems[index]);
        }
        EXPECT_FALSE(iter.hasNext());
    }
    template<typename T>
    void RemovePointTest(T *targetItems, int itemsCount, int removePoint) {
        Array<T> a;
        for (int i = 0; i < itemsCount; i++) {
            a.insert(targetItems[i]);
        }
        a.remove(removePoint);
        typename Array<T>::Iterator iter = a.iterator();
        for (int i = 0; iter.hasNext()&&i < a.size()-1; iter.next(), i++){
            int index = i >= removePoint ? i + 1 : i;
            EXPECT_EQ(iter.get(), targetItems[index]);
        }
        EXPECT_FALSE(iter.hasNext());
    }
};

//TEST(ArrayTests, TestName) {
//
//}

TEST_F(ArrayTests, Construction){
    ConstructTest<int>();
    ConstructTest<float>();
    ConstructTest<string>();
}
TEST_F(ArrayTests, Filling){
    FillTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6);
    FillTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6);
    FillTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4);
}
TEST_F(ArrayTests, InsertingRemoving){
    InsertRemoveTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6, 3);
    InsertRemoveTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6, 2);
    InsertRemoveTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4, 1);
}
TEST_F(ArrayTests, DirectIterator){
    DirectIteratorTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6);
    DirectIteratorTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6);
    DirectIteratorTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4);
}
TEST_F(ArrayTests, SelfAssigning){
    SelfAssigmentTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6);
    SelfAssigmentTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6);
    SelfAssigmentTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4);
}
TEST_F(ArrayTests, ReverseIterator){
    ReverseIteratorTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6);
    ReverseIteratorTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6);
    ReverseIteratorTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4);
}
TEST_F(ArrayTests, Assigning){
    AssigmentTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6);
    AssigmentTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6);
    AssigmentTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4);
}
TEST_F(ArrayTests, InsertingIndex){
    InsertPointTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6, 3, 100);
    InsertPointTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6, 2, 2.71);
    InsertPointTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4, 1, "secret");
}
TEST_F(ArrayTests, RemovingIndex){
    RemovePointTest<int>(new int[]{0, 1, 2, 3, 555, 9}, 6, 3);
    RemovePointTest<float>(new float[]{32.3, 47.99, 20.111, 3.14159, 1700.1, 9}, 6, 2);
    RemovePointTest<string>(new string[]{"testtesttest", "string test", "", "crashcrashcrash"}, 4, 1);
}
