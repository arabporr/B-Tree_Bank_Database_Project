#include <iostream>
#include <string>
using namespace std;

class BTreeNode
{
public:
	int degree;
	int cnt_keys;
	bool is_leaf;
	Customer** keys;
	BTreeNode** Children;

	BTreeNode(int deg, bool leaf)
	{
		degree = deg;
		is_leaf = leaf;
		keys = new Customer* [2*degree-1];
		Children = new BTreeNode* [2*degree];
		cnt_keys = 0;
	}

	BTreeNode* search(Customer* person) 
	{
		int pos = 0;
		while(pos < cnt_keys and person->asset > keys[pos]->asset)
		{
			pos++;
		}
		if (pos < cnt_keys and keys[pos]->asset == person->asset and keys[pos]->id == person->id)
		{
			return this;
		}
		if (is_leaf == true)
		{
			return NULL;
		}

		return Children[pos]->search(person);
	}

	void BorrowLeft(int index)
	{

		BTreeNode* child = Children[index];
		BTreeNode* sibling = Children[index-1];

		for (int i = child->cnt_keys - 1; i >= 0; i--)
		{
			child->keys[i+1] = child->keys[i];
		}
		if (child->is_leaf == false)
		{
			for(int i = child->cnt_keys; i >= 0; i--)
			{
				child->Children[i+1] = child->Children[i];
			}
		}

		child->keys[0] = keys[index-1];

		if(child->is_leaf == false)
		{
			child->Children[0] = sibling->Children[sibling->cnt_keys];
		}

		keys[index-1] = sibling->keys[sibling->cnt_keys-1];

		child->cnt_keys += 1;
		sibling->cnt_keys -= 1;

		return;
	}

	void BorrowRight(int index)
	{

		BTreeNode* child=Children[index];
		BTreeNode* sibling=Children[index+1];

		child->keys[(child->cnt_keys)] = keys[index];

		if (child->is_leaf == false)
		{
			child->Children[(child->cnt_keys)+1] = sibling->Children[0];
		}

		keys[index] = sibling->keys[0];

		for (int i = 1; i < sibling->cnt_keys; i++)
		{
			sibling->keys[i-1] = sibling->keys[i];
		}
		if (sibling->is_leaf == false)
		{
			for(int i = 1; i <= sibling->cnt_keys; i++)
			{
				sibling->Children[i-1] = sibling->Children[i];
			}
		}

		child->cnt_keys += 1;
		sibling->cnt_keys -= 1;

		return;
	}

	void merge_with_next(int index)
	{
		BTreeNode* child = Children[index];
		BTreeNode* sibling = Children[index+1];

		child->keys[degree-1] = keys[index];

		for (int i = 0; i < sibling->cnt_keys; i++)
		{
			child->keys[i+degree] = sibling->keys[i];
		}

		if (child->is_leaf == false)
		{
			for(int i = 0; i <= sibling->cnt_keys; i++)
			{
				child->Children[i+degree] = sibling->Children[i];
			}
		}

		for (int i=index+1; i < cnt_keys; i++)
		{
			keys[i-1] = keys[i];
		}

		for (int i=index+2; i <= cnt_keys; i++)
		{
			Children[i-1] = Children[i];
		}

		child->cnt_keys += sibling->cnt_keys + 1;
		cnt_keys --;

		return;
	}

	void fill(int child_index)
	{
		if (child_index != 0 and Children[child_index-1]->cnt_keys >= degree)
		{
			BorrowLeft(child_index);
		}
		else if (child_index != cnt_keys and Children[child_index+1]->cnt_keys >= degree)
		{
			BorrowRight(child_index );
		}
		else
		{
			if (child_index  != cnt_keys)
			{
				merge_with_next(child_index);
			}
			else
			{
				merge_with_next(child_index-1);
			}
		}
		return;
	}

	void remove(Customer* person)
	{
		int index=0;
		while (index < cnt_keys and keys[index]->asset < person->asset)
		{
			index++;
		}

		if (index < cnt_keys and keys[index]->asset == person->asset and keys[index]->id == person->id)
		{
			if(is_leaf == true)
			{
				for (int i = index+1; i < cnt_keys; i++)
				{
					keys[i-1] = keys[i];
				}
				cnt_keys--;
			}
			else
			{
				if (Children[index]->cnt_keys >= degree)
				{
					BTreeNode* current = Children[index];
					while (current->is_leaf == false)
					{
						current = current->Children[current->cnt_keys];
					}
					Customer* pred =  current->keys[current->cnt_keys-1];
					keys[index] = pred;
					Children[index]->remove(pred);
				}
				else if (Children[index+1]->cnt_keys >= degree)
				{
					BTreeNode* current = Children[index+1];
					while (current->is_leaf == false)
					{
						current = current->Children[0];
					}
					Customer* succ = current->keys[0];
					keys[index] = succ;
					Children[index+1]->remove(succ);
				}
				else
				{
					merge_with_next(index);
					Children[index]->remove(person);
				}
			}
		}
		else
		{
			if(is_leaf == true)
			{
				cout<<"INTERNAL ERROR !\n\tThe person with id : "<<person->id<<" and asset : "<<person->asset<<" does not exist in the tree"<<endl;
				return;
			}
			if (Children[index]->cnt_keys < degree)
			{
				fill(index);
			}
			if (index > cnt_keys)
			{
				Children[index-1]->remove(person);
			}
			else
			{
				Children[index]->remove(person);
			}
		}
		return;
	}

	void insert(Customer* person)
	{
		if (is_leaf == true)
		{
			int pos = cnt_keys-1;
			while (pos >= 0 and keys[pos]->asset > person->asset)
			{
				keys[pos+1] = keys[pos];
				pos--;
			}
			keys[pos+1] = person;
			cnt_keys = cnt_keys+1;
		}
		else
		{
			int pos = cnt_keys-1;
			while (pos >= 0 and keys[pos]->asset > person->asset)
				pos--;
			if (Children[pos+1]->cnt_keys == 2*degree-1)
			{
				int index = pos+1;
				BTreeNode* child = Children[pos+1];
				BTreeNode* new_child = new BTreeNode(child->degree, child->is_leaf);

				new_child->cnt_keys = degree - 1;
				for (int i = 0; i < degree - 1; i++)
				{
					new_child->keys[i] = child->keys[i+degree];
				}
				if (child->is_leaf == false)
				{
					for (int i = 0; i < degree; i++)
					{
						new_child->Children[i] = child->Children[i+degree];
					}
				}

				child->cnt_keys = degree - 1;
				for (int i = cnt_keys; i >= index+1; i--)
				{
					Children[i+1] = Children[i];
				}
				Children[index+1] = new_child;

				for (int i = cnt_keys-1; i >= index; i--)
					keys[i+1] = keys[i];

				keys[index] = child->keys[degree-1];
				cnt_keys = cnt_keys + 1;

				if (keys[pos+1] < person)
					pos++;
			}
			Children[pos+1]->insert(person);
		}
	}
};

class BTree
{
public:
	BTreeNode* root;
	int degree;
	BTree(int deg)
	{
		root = NULL;
		degree = deg;
	}

	void insert(Customer* person)
	{
		if (root == NULL)
		{
			root = new BTreeNode(degree, true);
			root->keys[0] = person;
			root->cnt_keys = 1;
		}
		else
		{
			if (root->cnt_keys == 2*degree-1)
			{
				BTreeNode* old_root = root;
				BTreeNode* new_root = new BTreeNode(degree, false);
				BTreeNode* new_child = new BTreeNode(old_root->degree, old_root->is_leaf);	
				
				new_root->Children[0] = old_root;
				new_root->Children[1] = new_child;
				new_root->keys[0] = old_root->keys[degree-1];
				new_root->cnt_keys = 1;
				old_root->cnt_keys = degree - 1;
				new_child->cnt_keys = degree - 1;

				for (int i = 0; i < degree - 1; i++)
				{
					new_child->keys[i] = old_root->keys[i+degree];
				}

				if (old_root->is_leaf == false)
				{
					for (int i = 0; i < degree; i++)
					{
						new_child->Children[i] = old_root->Children[i+degree];
					}
				}

				int pos = 0;
				if (new_root->keys[0]->asset < person->asset)
				{
					pos++;
				}
				new_root->Children[pos]->insert(person);

				root = new_root;
			}
			else
			{
				root->insert(person);
			}
		}
	}

	void remove(Customer* person)
	{
		if (root == NULL)
		{
			cout<<"Internal ERROR!\n\tRemoving from an empty B-Tree !"<<endl;
			return;
		}
		root->remove(person);

		if (root->cnt_keys == 0)
		{
			if (root->is_leaf)
			{
				root = NULL;
			}
			else
			{
				root = root->Children[0];
			}
		}
	}

	Customer* getRichest()
	{
		if(root == NULL)
		{
			return NULL;
		}
		else
		{
			BTreeNode* current = root;
			while (current->is_leaf == false)
			{
				current = current->Children[current->cnt_keys];
			}
			return current->keys[current->cnt_keys-1];
		}
	}
};
