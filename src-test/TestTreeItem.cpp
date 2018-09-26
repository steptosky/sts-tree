/*
**  Copyright(C) 2018, StepToSky
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

#include <memory>
#include <algorithm>
#include "gtest/gtest.h"
#include "sts/tree/TreeItem.h"

using namespace sts::tree;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class TestTreeItem : public TreeItem<TestTreeItem> {

    typedef TreeItem<TestTreeItem> Base;

public:

    static int instances;
    int mMark;

    TestTreeItem * clone() const override {
        return new TestTreeItem(*this);
    }

    explicit TestTreeItem(const int mark = -1)
        : mMark(mark) {
        ++instances;
    }

    explicit TestTreeItem(TestTreeItem * item, const int mark = -1)
        : Base(item),
          mMark(mark) {
        ++instances;
    }

    ~TestTreeItem() {
        --instances;
    }

};

int TestTreeItem::instances = 0;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, isRoot) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->prependChild(new TestTreeItem);
    //---------------------------------
    ASSERT_TRUE(treeRoot->isRoot());
    ASSERT_FALSE(tree0->isRoot());
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, root) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    auto tree0 = treeRoot->appendChild(new TestTreeItem);
    auto tree1 = treeRoot->appendChild(new TestTreeItem);
    auto tree2 = treeRoot->appendChild(new TestTreeItem);
    auto tree3 = tree2->appendChild(new TestTreeItem);
    auto tree4 = tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    ASSERT_TRUE(tree0->root() == treeRoot.get());
    ASSERT_TRUE(tree1->root() == treeRoot.get());
    ASSERT_TRUE(tree2->root() == treeRoot.get());
    ASSERT_TRUE(tree3->root() == treeRoot.get());
    ASSERT_TRUE(tree4->root() == treeRoot.get());
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, setParent) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    auto tree0 = new TestTreeItem;
    tree0->setParent(treeRoot.get());
    //---------------------------------
    ASSERT_TRUE(treeRoot->parent() == nullptr);
    ASSERT_TRUE(tree0->parent() == treeRoot.get());
    ASSERT_EQ(1, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, change_parent) {
    std::unique_ptr<TestTreeItem> treeRoot1 = std::make_unique<TestTreeItem>();
    std::unique_ptr<TestTreeItem> treeRoot2 = std::make_unique<TestTreeItem>();
    auto tree0 = new TestTreeItem;
    //---------------------------------
    tree0->setParent(treeRoot1.get());
    ASSERT_TRUE(tree0->parent() == treeRoot1.get());
    ASSERT_EQ(1, treeRoot1->childrenCount());
    ASSERT_TRUE(treeRoot1->childAt(0) == tree0);
    //---------------------------------
    tree0->setParent(treeRoot2.get());

    ASSERT_FALSE(tree0->parent() == treeRoot1.get());
    ASSERT_EQ(0, treeRoot1->childrenCount());

    ASSERT_TRUE(tree0->parent() == treeRoot2.get());
    ASSERT_EQ(1, treeRoot2->childrenCount());
    ASSERT_TRUE(treeRoot2->childAt(0) == tree0);
    //---------------------------------
    tree0->setParent(nullptr);
    ASSERT_TRUE(tree0->parent() == nullptr);
    EXPECT_EQ(0, treeRoot1->childrenCount());
    EXPECT_EQ(0, treeRoot2->childrenCount());
    delete tree0;
    //---------------------------------
    treeRoot1.reset();
    treeRoot2.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, parent) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_TRUE(treeRoot->parent() == nullptr);
    ASSERT_TRUE(tree0->parent() == treeRoot.get());
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, operator_access_by_index) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    const auto tree1 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_TRUE((*treeRoot)[0] == tree0);
    ASSERT_TRUE((*treeRoot)[1] == tree1);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, childAt) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    const auto tree1 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree1);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, takeChildAt) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    const auto tree2 = treeRoot->appendChild(new TestTreeItem);
    ASSERT_EQ(4, TestTreeItem::instances);
    //---------------------------------
    ASSERT_EQ(3, treeRoot->childrenCount());
    auto tree1 = treeRoot->takeChildAt(1);
    ASSERT_EQ(2, treeRoot->childrenCount());
    EXPECT_TRUE(tree1->parent() == nullptr);
    EXPECT_TRUE(treeRoot->childAt(0) == tree0);
    EXPECT_TRUE(treeRoot->childAt(1) == tree2);
    EXPECT_TRUE(treeRoot->indexOf(tree1) == treeRoot->npos);
    treeRoot.reset();
    EXPECT_EQ(1, TestTreeItem::instances);
    delete tree1;
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, prepend) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    auto tree0 = treeRoot->prependChild(new TestTreeItem);
    auto tree1 = treeRoot->prependChild(new TestTreeItem);
    auto tree2 = treeRoot->prependChild(new TestTreeItem);
    auto tree3 = treeRoot->prependChild(new TestTreeItem);
    auto tree4 = treeRoot->prependChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree4);
    ASSERT_TRUE(treeRoot->childAt(1) == tree3);
    ASSERT_TRUE(treeRoot->childAt(2) == tree2);
    ASSERT_TRUE(treeRoot->childAt(3) == tree1);
    ASSERT_TRUE(treeRoot->childAt(4) == tree0);
    ASSERT_TRUE(tree0->parent() == treeRoot.get());
    ASSERT_TRUE(tree1->parent() == treeRoot.get());
    ASSERT_TRUE(tree2->parent() == treeRoot.get());
    ASSERT_TRUE(tree3->parent() == treeRoot.get());
    ASSERT_TRUE(tree4->parent() == treeRoot.get());
    ASSERT_EQ(6, TestTreeItem::instances);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, append) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    auto tree0 = treeRoot->appendChild(new TestTreeItem);
    auto tree1 = treeRoot->appendChild(new TestTreeItem);
    auto tree2 = treeRoot->appendChild(new TestTreeItem);
    auto tree3 = treeRoot->appendChild(new TestTreeItem);
    auto tree4 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree1);
    ASSERT_TRUE(treeRoot->childAt(2) == tree2);
    ASSERT_TRUE(treeRoot->childAt(3) == tree3);
    ASSERT_TRUE(treeRoot->childAt(4) == tree4);
    ASSERT_TRUE(tree0->parent() == treeRoot.get());
    ASSERT_TRUE(tree1->parent() == treeRoot.get());
    ASSERT_TRUE(tree2->parent() == treeRoot.get());
    ASSERT_TRUE(tree3->parent() == treeRoot.get());
    ASSERT_TRUE(tree4->parent() == treeRoot.get());
    ASSERT_EQ(6, TestTreeItem::instances);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, insert) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    auto tree0 = treeRoot->appendChild(new TestTreeItem);
    auto tree1 = treeRoot->appendChild(new TestTreeItem);
    auto tree2 = treeRoot->insertChild(1, new TestTreeItem);
    auto tree3 = treeRoot->insertChild(2, new TestTreeItem);
    auto tree4 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(5, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree2);
    ASSERT_TRUE(treeRoot->childAt(2) == tree3);
    ASSERT_TRUE(treeRoot->childAt(3) == tree1);
    ASSERT_TRUE(treeRoot->childAt(4) == tree4);
    ASSERT_TRUE(tree0->parent() == treeRoot.get());
    ASSERT_TRUE(tree1->parent() == treeRoot.get());
    ASSERT_TRUE(tree2->parent() == treeRoot.get());
    ASSERT_TRUE(tree3->parent() == treeRoot.get());
    ASSERT_TRUE(tree4->parent() == treeRoot.get());
    ASSERT_EQ(6, TestTreeItem::instances);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, deleteChildren) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    auto tree2 = treeRoot->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    treeRoot->deleteChildren();
    ASSERT_EQ(1, TestTreeItem::instances);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, deleteChildAt) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    const auto tree2 = treeRoot->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    //---------------------------------
    treeRoot->deleteChildAt(1);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree2);
    ASSERT_EQ(3, TestTreeItem::instances);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, deleteChild) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    const auto tree1 = treeRoot->appendChild(new TestTreeItem);
    const auto tree2 = treeRoot->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    //---------------------------------
    ASSERT_TRUE(treeRoot->deleteChild(tree1));
    ASSERT_FALSE(treeRoot->deleteChild(tree1));
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_TRUE(treeRoot->childAt(0) == tree0);
    ASSERT_TRUE(treeRoot->childAt(1) == tree2);
    ASSERT_EQ(3, TestTreeItem::instances);
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, hasChildren) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    treeRoot->appendChild(new TestTreeItem);
    treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(3, TestTreeItem::instances);
    ASSERT_TRUE(treeRoot->hasChildren());
    //---------------------------------
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, indexOf) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    const auto tree1 = treeRoot->appendChild(new TestTreeItem);
    const auto tree2 = treeRoot->appendChild(new TestTreeItem);
    const auto tree3 = treeRoot->appendChild(new TestTreeItem);
    const auto tree4 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_EQ(6, TestTreeItem::instances);

    ASSERT_EQ(TestTreeItem::Index(0), treeRoot->indexOf(tree0));
    ASSERT_EQ(TestTreeItem::Index(1), treeRoot->indexOf(tree1));
    ASSERT_EQ(TestTreeItem::Index(2), treeRoot->indexOf(tree2));
    ASSERT_EQ(TestTreeItem::Index(3), treeRoot->indexOf(tree3));
    ASSERT_EQ(TestTreeItem::Index(4), treeRoot->indexOf(tree4));
    //---------------------------------
    delete tree1;
    delete tree3;
    ASSERT_EQ(4, TestTreeItem::instances);
    ASSERT_EQ(TestTreeItem::Index(0), treeRoot->indexOf(tree0));
    ASSERT_EQ(treeRoot->npos, treeRoot->indexOf(tree1));
    ASSERT_EQ(TestTreeItem::Index(1), treeRoot->indexOf(tree2));
    ASSERT_EQ(treeRoot->npos, treeRoot->indexOf(tree3));
    ASSERT_EQ(TestTreeItem::Index(2), treeRoot->indexOf(tree4));
    ASSERT_EQ(3, treeRoot->childrenCount());

    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, isBranch_isLeaf) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    //---------------------------------
    ASSERT_TRUE(treeRoot->isBranch());
    ASSERT_TRUE(tree0->isLeaf());

    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

TEST(TestTreeItem, isChildOf) {
    std::unique_ptr<TestTreeItem> treeRoot0 = std::make_unique<TestTreeItem>();
    auto tree1 = treeRoot0->appendChild(new TestTreeItem);
    const auto tree2 = tree1->appendChild(new TestTreeItem);

    std::unique_ptr<TestTreeItem> treeRoot1 = std::make_unique<TestTreeItem>();
    auto tree4 = treeRoot1->appendChild(new TestTreeItem);
    const auto tree5 = tree4->appendChild(new TestTreeItem);

    ASSERT_TRUE(tree1->isChildOf(treeRoot0.get()));
    ASSERT_TRUE(tree4->isChildOf(treeRoot1.get()));
    ASSERT_FALSE(tree1->isChildOf(treeRoot1.get()));
    ASSERT_FALSE(tree4->isChildOf(treeRoot0.get()));

    ASSERT_TRUE(tree2->isChildOf(tree1));
    ASSERT_TRUE(tree5->isChildOf(tree4));
    ASSERT_FALSE(tree2->isChildOf(tree4));
    ASSERT_FALSE(tree5->isChildOf(tree1));

    ASSERT_TRUE(tree2->isChildOf(treeRoot0.get()));
    ASSERT_TRUE(tree5->isChildOf(treeRoot1.get()));
    ASSERT_FALSE(tree2->isChildOf(treeRoot1.get()));
    ASSERT_FALSE(tree5->isChildOf(treeRoot0.get()));

    treeRoot0.reset();
    treeRoot1.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

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
TEST(TestTreeItem, re_parenting) {
    //---------------------------------
    // 1
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    auto tree1 = treeRoot->appendChild(new TestTreeItem);
    auto tree2 = treeRoot->appendChild(new TestTreeItem);
    tree2->appendChild(new TestTreeItem);
    TestTreeItem * tree4 = tree2->appendChild(new TestTreeItem);
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    //---------------------------------
    // 2
    tree2->setParent(nullptr);
    ASSERT_TRUE(tree2->parent() == nullptr);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    //---------------------------------
    // 3
    tree2->setParent(tree0);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(1, tree0->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    ASSERT_FALSE(tree0->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    //---------------------------------
    // 4
    tree1->appendChild(tree2);
    ASSERT_EQ(2, treeRoot->childrenCount());
    ASSERT_EQ(2, tree2->childrenCount());
    ASSERT_EQ(0, tree0->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    ASSERT_FALSE(tree1->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    //---------------------------------
    // 5
    tree4->setParent(treeRoot.get());
    ASSERT_EQ(3, treeRoot->childrenCount());
    ASSERT_EQ(1, tree2->childrenCount());
    ASSERT_EQ(6, TestTreeItem::instances);
    ASSERT_FALSE(tree1->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(treeRoot->indexOf(tree2) == treeRoot->npos);
    ASSERT_TRUE(tree2->indexOf(tree4) == treeRoot->npos);
    //---------------------------------
    treeRoot.reset();
    ASSERT_EQ(0, TestTreeItem::instances);
    //---------------------------------
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

TEST(TestTreeItem, iteration_farward) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    const auto tree1 = treeRoot->appendChild(new TestTreeItem);
    const auto tree2 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem::Children check;
    //---------------------------------
    for (auto child : *treeRoot) {
        ASSERT_TRUE(child->parent() == treeRoot.get());
        check.emplace_back(child);
    }
    //---------------------------------
    ASSERT_EQ(3, check.size());
    EXPECT_TRUE(tree0 == check[0]);
    EXPECT_TRUE(tree1 == check[1]);
    EXPECT_TRUE(tree2 == check[2]);
}

TEST(TestTreeItem, iteration_reverse) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem);
    const auto tree1 = treeRoot->appendChild(new TestTreeItem);
    const auto tree2 = treeRoot->appendChild(new TestTreeItem);
    TestTreeItem::Children check;
    //---------------------------------
    for (auto child = treeRoot->rbegin(); child != treeRoot->rend(); ++child) {
        ASSERT_TRUE((*child)->parent() == treeRoot.get());
        check.emplace_back(*child);
    }
    //---------------------------------
    ASSERT_EQ(3, check.size());
    EXPECT_TRUE(tree2 == check[0]);
    EXPECT_TRUE(tree1 == check[1]);
    EXPECT_TRUE(tree0 == check[2]);
}

TEST(TestTreeItem, sorting) {
    std::unique_ptr<TestTreeItem> treeRoot = std::make_unique<TestTreeItem>();
    const auto tree0 = treeRoot->appendChild(new TestTreeItem(2));
    const auto tree1 = treeRoot->appendChild(new TestTreeItem(1));
    const auto tree2 = treeRoot->appendChild(new TestTreeItem(0));
    //---------------------------------
    ASSERT_EQ(3, treeRoot->childrenCount());
    EXPECT_TRUE(tree0 == (*treeRoot)[0]);
    EXPECT_TRUE(tree1 == (*treeRoot)[1]);
    EXPECT_TRUE(tree2 == (*treeRoot)[2]);
    //---------------------------------
    std::sort(treeRoot->begin(), treeRoot->end(), [](const auto val1, const auto val2) {
        return val1->mMark < val2->mMark;
    });
    //---------------------------------
    ASSERT_EQ(3, treeRoot->childrenCount());
    EXPECT_TRUE(tree0 == (*treeRoot)[2]);
    EXPECT_TRUE(tree1 == (*treeRoot)[1]);
    EXPECT_TRUE(tree2 == (*treeRoot)[0]);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
