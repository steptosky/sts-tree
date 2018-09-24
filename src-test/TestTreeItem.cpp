/*
**  Copyright(C) 2017, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com
*/

#include "gtest/gtest.h"
#include "sts/tree/TreeItem.h"

//#define PERFORMANCE_TEST
#ifdef _DEBUG
#define TEST_COUNT 2000
#else
#define TEST_COUNT 50000
#endif // _DEBUG

using namespace sts::tree;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class TestTreeItem : public TreeItem<TestTreeItem, TreeItemContainerVector<TestTreeItem>> {
    typedef TreeItem<TestTreeItem, TreeItemContainerVector<TestTreeItem>> Base;
public:

    static int instanceCreated;
    int mMark;

    TestTreeItem & operator =(const TestTreeItem & input) {
        Base::operator=(input);
        mMark = input.mMark;
        return *this;
    }

    TestTreeItem * clone() const override {
        return new TestTreeItem(*this);
    }

    TestTreeItem(int inMark = -1)
        : mMark(inMark) {
        ++instanceCreated;
    }

    TestTreeItem(TestTreeItem * inTreeItem, int inMark = -1)
        : Base(inTreeItem),
          mMark(inMark) {
        ++instanceCreated;
    }

    TestTreeItem(const TestTreeItem & inTreeItem)
        : Base(inTreeItem),
          mMark(inTreeItem.mMark) {
        ++instanceCreated;
    }

    ~TestTreeItem() {
        --instanceCreated;
    }

};

int TestTreeItem::instanceCreated = 0;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
#ifdef PERFORMANCE_TEST

TEST(TestTreeItem, prependPerformance){
//---------------------------------
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->prependChild(new TestTreeItem());
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, appendPerformance){
//---------------------------------
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->appendChild(new TestTreeItem());
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, insertPerformance){
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT / 2; ++i)
        treeRoot->appendChild(new TestTreeItem());
//---------------------------------
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->insertChild(TEST_COUNT / 2, new TestTreeItem());
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1 + (TEST_COUNT / 2), TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, deleteRootPerformance){
//---------------------------------
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->appendChild(new TestTreeItem());
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}


TEST(TestTreeItem, deleteChildrenByPerformance){
//---------------------------------
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->appendChild(new TestTreeItem());
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1, TestTreeItem::instanceCreated);
    const TestTreeItem::Children &children = treeRoot->children();
    for (size_t i = 0; i < children.size(); ++i){
        delete treeRoot->childAt(i);
        --i;
    }
    ASSERT_EQ(1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, deleteAllChildrenPerformance){
//---------------------------------
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->appendChild(new TestTreeItem());
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1, TestTreeItem::instanceCreated);
    treeRoot->deleteChildren();
    ASSERT_EQ(1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, iteratePerformance){
//---------------------------------
    TestTreeItem *treeRoot = new TestTreeItem();
    for (size_t i = 0; i < TEST_COUNT; ++i)
        treeRoot->appendChild(new TestTreeItem());
//---------------------------------
    int i = 0;
//---------------------------------
    for (auto &item : treeRoot->children()){
        if (item == nullptr)
            continue;
        item->mMark = i;
        ++i;
    }
//---------------------------------
    ASSERT_EQ(TEST_COUNT + 1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

#endif // PERFORMANCE_TEST
/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, prepend) {
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->prependChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->prependChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->prependChild(new TestTreeItem);
    TestTreeItem * tree3 = treeRoot->prependChild(new TestTreeItem);
    TestTreeItem * tree4 = treeRoot->prependChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree4);
    ASSERT_TRUE(treeRoot->childAt(1) == tree3);
    ASSERT_TRUE(treeRoot->childAt(2) == tree2);
    ASSERT_TRUE(treeRoot->childAt(3) == tree1);
    ASSERT_TRUE(treeRoot->childAt(4) == tree0);
    ASSERT_TRUE(tree0->parent() == treeRoot);
    ASSERT_TRUE(tree1->parent() == treeRoot);
    ASSERT_TRUE(tree2->parent() == treeRoot);
    ASSERT_TRUE(tree3->parent() == treeRoot);
    ASSERT_TRUE(tree4->parent() == treeRoot);
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, append) {
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree3 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree4 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree1);
    ASSERT_TRUE(treeRoot->childAt(2) == tree2);
    ASSERT_TRUE(treeRoot->childAt(3) == tree3);
    ASSERT_TRUE(treeRoot->childAt(4) == tree4);
    ASSERT_TRUE(tree0->parent() == treeRoot);
    ASSERT_TRUE(tree1->parent() == treeRoot);
    ASSERT_TRUE(tree2->parent() == treeRoot);
    ASSERT_TRUE(tree3->parent() == treeRoot);
    ASSERT_TRUE(tree4->parent() == treeRoot);
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, insert) {
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->insertChild(1, new TestTreeItem);
    TestTreeItem * tree3 = treeRoot->insertChild(2, new TestTreeItem);
    TestTreeItem * tree4 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree2);
    ASSERT_TRUE(treeRoot->childAt(2) == tree3);
    ASSERT_TRUE(treeRoot->childAt(3) == tree1);
    ASSERT_TRUE(treeRoot->childAt(4) == tree4);
    ASSERT_TRUE(tree0->parent() == treeRoot);
    ASSERT_TRUE(tree1->parent() == treeRoot);
    ASSERT_TRUE(tree2->parent() == treeRoot);
    ASSERT_TRUE(tree3->parent() == treeRoot);
    ASSERT_TRUE(tree4->parent() == treeRoot);
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, taking) {
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    //---------------------------------
    tree1 = treeRoot->takeChildAt(1);
    ASSERT_TRUE(tree1->parent() == nullptr);
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree2);
    ASSERT_TRUE(treeRoot->indexOf(tree1) == treeRoot->npos);
    delete treeRoot;
    ASSERT_EQ(1, TestTreeItem::instanceCreated);
    tree1->setParent(nullptr); // test for correct pointer
    delete tree1;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, indexOf) {
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree3 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree4 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(6, TestTreeItem::instanceCreated);

    ASSERT_EQ(TestTreeItem::Index(0), treeRoot->indexOf(tree0));
    ASSERT_EQ(TestTreeItem::Index(1), treeRoot->indexOf(tree1));
    ASSERT_EQ(TestTreeItem::Index(2), treeRoot->indexOf(tree2));
    ASSERT_EQ(TestTreeItem::Index(3), treeRoot->indexOf(tree3));
    ASSERT_EQ(TestTreeItem::Index(4), treeRoot->indexOf(tree4));
    //---------------------------------
    delete tree1;
    delete tree3;
    ASSERT_EQ(4, TestTreeItem::instanceCreated);
    ASSERT_EQ(TestTreeItem::Index(0), treeRoot->indexOf(tree0));
    ASSERT_EQ(treeRoot->npos, treeRoot->indexOf(tree1));
    ASSERT_EQ(TestTreeItem::Index(1), treeRoot->indexOf(tree2));
    ASSERT_EQ(treeRoot->npos, treeRoot->indexOf(tree3));
    ASSERT_EQ(TestTreeItem::Index(2), treeRoot->indexOf(tree4));
    ASSERT_EQ(3, treeRoot->childrenCount());

    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, deleteRoot) {
    TestTreeItem * treeRoot = new TestTreeItem();
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, deleteChildren) {
    TestTreeItem * treeRoot = new TestTreeItem();
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    treeRoot->deleteChildren();
    ASSERT_EQ(1, TestTreeItem::instanceCreated);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, deleteChildByChild) {
    TestTreeItem * treeRoot = new TestTreeItem();
    treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);

    tree2->appendChild(new TestTreeItem);
    TestTreeItem * tree4 = tree2->appendChild(new TestTreeItem);

    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);

    treeRoot->deleteChild(1);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(5, TestTreeItem::instanceCreated);
    ASSERT_TRUE(treeRoot->indexOf(tree1) == treeRoot->npos);

    tree2->deleteChild(1);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(1, tree2->childrenCount());
    ASSERT_EQ(4, TestTreeItem::instanceCreated);
    ASSERT_TRUE(tree2->indexOf(tree4) == treeRoot->npos);

    treeRoot->deleteChild(1);
    ASSERT_EQ(1, treeRoot->childrenCount());
    ASSERT_EQ(2, TestTreeItem::instanceCreated);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);

    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, getRoot) {
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree3 = tree2->appendChild(new TestTreeItem);
    TestTreeItem * tree4 = tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    ASSERT_TRUE(tree0->root() == treeRoot);
    ASSERT_TRUE(tree1->root() == treeRoot);
    ASSERT_TRUE(tree2->root() == treeRoot);
    ASSERT_TRUE(tree3->root() == treeRoot);
    ASSERT_TRUE(tree4->root() == treeRoot);
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, isChildOf) {
    TestTreeItem * treeRoot0 = new TestTreeItem();
    TestTreeItem * tree1 = treeRoot0->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = tree1->appendChild(new TestTreeItem);

    TestTreeItem * treeRoot1 = new TestTreeItem();
    TestTreeItem * tree4 = treeRoot1->appendChild(new TestTreeItem);
    TestTreeItem * tree5 = tree4->appendChild(new TestTreeItem);

    ASSERT_TRUE(tree1->isChildOf(treeRoot0));
    ASSERT_TRUE(tree4->isChildOf(treeRoot1));
    ASSERT_FALSE(tree1->isChildOf(treeRoot1));
    ASSERT_FALSE(tree4->isChildOf(treeRoot0));

    ASSERT_TRUE(tree2->isChildOf(tree1));
    ASSERT_TRUE(tree5->isChildOf(tree4));
    ASSERT_FALSE(tree2->isChildOf(tree4));
    ASSERT_FALSE(tree5->isChildOf(tree1));

    ASSERT_TRUE(tree2->isChildOf(treeRoot0));
    ASSERT_TRUE(tree5->isChildOf(treeRoot1));
    ASSERT_FALSE(tree2->isChildOf(treeRoot1));
    ASSERT_FALSE(tree5->isChildOf(treeRoot0));

    delete treeRoot0;
    delete treeRoot1;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, copyOperator_inNew) {
    TestTreeItem * treeRoot = new TestTreeItem();
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);

    TestTreeItem * treeForCopy = new TestTreeItem();
    *treeForCopy = *treeRoot;
    ASSERT_EQ(5, treeForCopy->childrenCount());
    ASSERT_EQ(12, TestTreeItem::instanceCreated);
    ASSERT_TRUE(treeForCopy->parent() == nullptr);
    ASSERT_TRUE(treeRoot->childAt(0) != treeForCopy->childAt(0));
    ASSERT_TRUE(treeRoot->childAt(1) != treeForCopy->childAt(1));
    ASSERT_TRUE(treeRoot->childAt(2) != treeForCopy->childAt(2));
    ASSERT_TRUE(treeRoot->childAt(3) != treeForCopy->childAt(3));
    ASSERT_TRUE(treeRoot->childAt(4) != treeForCopy->childAt(4));

    delete treeRoot;
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    delete treeForCopy;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

TEST(TestTreeItem, copyConstructor_inNew) {
    TestTreeItem * treeRoot = new TestTreeItem();
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);

    TestTreeItem * treeForCopy = new TestTreeItem(*treeRoot);
    ASSERT_EQ(5, treeForCopy->childrenCount());
    ASSERT_EQ(12, TestTreeItem::instanceCreated);
    ASSERT_TRUE(treeForCopy->parent() == nullptr);
    ASSERT_TRUE(treeRoot->childAt(0) != treeForCopy->childAt(0));
    ASSERT_TRUE(treeRoot->childAt(1) != treeForCopy->childAt(1));
    ASSERT_TRUE(treeRoot->childAt(2) != treeForCopy->childAt(2));
    ASSERT_TRUE(treeRoot->childAt(3) != treeForCopy->childAt(3));
    ASSERT_TRUE(treeRoot->childAt(4) != treeForCopy->childAt(4));

    delete treeRoot;
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    delete treeForCopy;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

//----------------------------------------------
//                    +root
//  +root                 tree0   
//      tree0             tree1
//      tree1            +tree2
//     +tree2                 tree0_copy 
//          tree3             tree1_copy
//          tree4            +tree2_copy
//                                tree3_copy
//                                tree4_copy
//    tree2 = *root;    //
//----------------------------------------------
TEST(TestTreeItem, copyOperator_inExist) {
    // Now constructor and operator copy are using same function inside and they are equivalent.
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem(0));
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem(1));
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem(2));

    TestTreeItem * tree3 = tree2->appendChild(new TestTreeItem(3));
    TestTreeItem * tree4 = tree2->appendChild(new TestTreeItem(4));

    ASSERT_EQ(0, tree0->mMark);
    ASSERT_EQ(1, tree1->mMark);
    ASSERT_EQ(2, tree2->mMark);
    ASSERT_EQ(3, tree3->mMark);
    ASSERT_EQ(4, tree4->mMark);

    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    //---------------------------------
    *tree2 = *treeRoot;
    ASSERT_EQ(9, TestTreeItem::instanceCreated);
    ASSERT_TRUE(tree2->parent() == treeRoot);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, treeRoot->indexOf(tree2));
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree1);
    ASSERT_TRUE(treeRoot->childAt(2) == tree2);
    ASSERT_EQ(3, tree2->childrenCount());
    ASSERT_EQ(2, tree2->childAt(2)->childrenCount());
    //---------------------------------
    ASSERT_EQ(0, tree0->mMark);
    ASSERT_EQ(1, tree1->mMark);

    ASSERT_EQ(-1, tree2->mMark); // -1 is copied from the root
    ASSERT_EQ(3, tree2->childrenCount());
    ASSERT_EQ(0, tree2->childAt(0)->mMark);
    ASSERT_EQ(1, tree2->childAt(1)->mMark);
    ASSERT_EQ(2, tree2->childAt(2)->mMark); // this value depends how the operator is implemented.
    ASSERT_EQ(2, tree2->childAt(2)->childrenCount());
    ASSERT_EQ(3, tree2->childAt(2)->childAt(0)->mMark);
    ASSERT_EQ(4, tree2->childAt(2)->childAt(1)->mMark);
    //---------------------------------
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
}

//-------------------------------------------------------------------------------------------
//
//  +root         | +root      | +root            | +root              | +root    
//      tree0     |     tree0  |    +tree0        |     tree0          |     tree0
//      tree1     |     tree1  |       +tree2     |    +tree1          |    +tree1
//     +tree2     | +tree2     |           tree3  |        +tree2      |        +tree2   
//          tree3 |     tree3  |           tree4  |             tree3  |             tree3
//          tree4 |     tree4  |     tree1        |             tree4  |     tree4
//
//-------------------------------------------------------------------------------------------
TEST(TestTreeItem, parenting) {
    //---------------------------------
    // 1
    TestTreeItem * treeRoot = new TestTreeItem();
    TestTreeItem * tree0 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree1 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem * tree2 = treeRoot->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    TestTreeItem * tree4 = tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    //---------------------------------
    // 2
    tree2->setParent(nullptr);
    ASSERT_TRUE(tree2->parent() == nullptr);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    //---------------------------------
    // 3
    tree2->setParent(tree0);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(1, tree0->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    ASSERT_FALSE(tree0->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    //---------------------------------
    // 4
    tree1->appendChild(tree2);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(0, tree0->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    ASSERT_FALSE(tree1->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    //---------------------------------
    // 5
    tree4->setParent(treeRoot);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(1, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instanceCreated);
    ASSERT_FALSE(tree1->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(tree2->indexOf(tree4) == treeRoot->npos);
    //---------------------------------
    delete treeRoot;
    ASSERT_EQ(0, TestTreeItem::instanceCreated);
    //---------------------------------
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
