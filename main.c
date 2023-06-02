#include <stdio.h>
#define Data int
#define max(a,b) (((a)>(b))?(a):(b))

// ��� ����
typedef struct _node {
	Data data;
	struct _node* left, * right;
	int height;
} Node;

// ��� ����
Node* new_node(Data data) {
	Node* node = malloc(sizeof(*node));
	if (node == NULL) {
		exit(1);
	}
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}

// ����� ���� ��ȯ�ϴ� �Լ�
int get_height(Node* node) {
	if (node == NULL) {
		return 0;
	}
	return node->height;
}

// ����� ���� ����ϴ� �Լ� 
void set_height(Node* node) {
	node->height = 1 + max(get_height(node->left), get_height(node->right));
}

// ����/������ ����Ʈ���� ���� ���̸� ���ϴ� �Լ� 
int diff_height(Node* node) {
	if (node == NULL) {
		return 0;
	}
	return get_height(node->left) - get_height(node->right);
}

// right subtree of right child -> single left rotation
Node* left_rotate(Node* node) {
	Node* right_child = node->right;
	
	//����� �����ʿ� �������ڽ��� ���� ��带 ����, ������ �ڽ��� ���ʿ� ��带 ���´�.
	node->right = right_child->left;
	right_child->left = node;
	
	// rotation �� ���� ����
	set_height(node);
	return right_child;
}

// left subtree of left child -> single right rotation
Node* right_rotate(Node* node) {
	Node* left_child = node->left;
	
	//����� ���ʿ� �����ڽ��� ������ ��带 ����, ���� �ڽ��� �����ʿ� ��带 ���´�. 
	node->left = left_child->right;
	left_child->right = node;
	
	// rotation �� ���� ����
	set_height(node);
	return left_child;
}

// Ʈ���� ���� �����ϴ� �Լ�
Node* reblalacing(Node* node)
{
	int h = diff_height(node);
	if (h > 1) { // ���� ����Ʈ���� ���̰� �� ũ�� 
		h = diff_height(node->left);
		// LL 
		if (h >= 0) {
			return right_rotate(node);
		}
		// LR: inside case -> double rotation
		else if (h < 0) {
			node->left = left_rotate(node->left);
			return right_rotate(node);
		}
	}
	else if (h < -1) { // ������ ����Ʈ���� ���̰� �� ũ��
		h = diff_height(node->right);
		// RR 
		if (h <= 0) {
			return left_rotate(node);
		}
		// RL: inside case -> double rotation 
		else if (h > 0) {
			node->right = right_rotate(node->right);
			return left_rotate(node);
		}
	}
	set_height(node);
	return node;
}

// ��� �����ϴ� �Լ�
Node* insert_node(Node* node, Data data) {
	if (node == NULL) {
		node = new_node(data);
		return node;
	}
	if (data > node->data) { // data�� ��庸�� ũ�� ����� �����ʿ� ����
		node->right = insert_node(node->right, data);
	}
	else if (data < node->data) { // data�� ��庸�� ������ ����� ���ʿ� ����
		node->left = insert_node(node->left, data);
	}
	else
		return node;

	// ���� �� Ʈ���� ���� ���
	node = reblalacing(node);
	return node;
}

// Ʈ�� Ž���ϴ� �Լ�
Node* search_node(Node* node, Data data) {
	if (node == NULL) { // ���� �� ã�� ���
		return NULL;
	}
	if (data == node->data) { // ���� ã�� ���
		return node;
	}
	else if (data > node->data) { //���� ��庸�� ū ��� ������ ����Ʈ�� Ž��
		search_node(node->right, data);
	}
	else { // ���� ��庸�� ���� ��� ���� ����Ʈ�� Ž��
		search_node(node->left, data);
	}
}


// ���� ������ ���, �ּҰ��� �������� �Լ�
Node* min_value_node(Node* node) {
	Node* current = node;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

// ��� �����ϴ� �Լ�
Node* delete_node(Node* node, Data data) {
	// base case  
	if (node == NULL)
		return node;

	// ������ ��带 ã�´�.    
	if (data < node->data) {
		node->left = delete_node(node->left, data);
	}

	else if (data > node->data) {
		node->right = delete_node(node->right, data);
	}
	// ������ ��带 ã�� ���
	else {
		Node* temp;
		// ��忡 �ڽ��� �ϳ� �ְų� ���� ���
		if (node->left == NULL) {
			temp = node->right;
			free(node);
			return temp;
		}
		else if (node->right == NULL) {
			temp = node->left;
			free(node);
			return temp;
		}

		// ��忡 �ڽ��� �� �ִ� ���
		// �����ϰ��� �ϴ� Ű������ ū ���� �� �ּҰ��� ���� ��带 ã��
		temp = min_value_node(node->right);
		// ã�� ����� Ű���� �����ϰ��� �ϴ� ��忡 ��ü
		node->data = temp->data;
		// ��ü �Ͽ������� ã�� ��� ����
		node->right = delete_node(node->right, temp->data);
	}


	node = reblalacing(node);

	return node;
}

// avl Ʈ���� ����ϴ� �Լ� 
void print_avl(Node* node, int level) {
	if (node != NULL) {
		// ���� ������ ������ �湮 �� ���� ��� �湮
		print_avl(node->right, level + 1);
		printf("\n");
		if (level == 1) {
			printf("root: ");
		}
		for (int i = 0; i < level && level != 1; i++) {
			printf("	");
		}
		printf("%d(%d)", node->data, get_height(node));
		print_avl(node->left, level + 1);
	}
}


int main() {
	Node* root = NULL;
	Data nums[] = {3,2,5,4,6,1,7};
	int len = sizeof(nums) / sizeof(int);

	for (int i = 0; i < len; i++) {
		root = insert_node(root, nums[i]);
	}
	print_avl(root, 1);
	printf("\n\n");

	root = delete_node(root, 1);
	print_avl(root, 1);
	printf("\n\n");

	return 0;
}