#include <stdio.h>
#define Data int
#define max(a,b) (((a)>(b))?(a):(b))

// 노드 구현
typedef struct _node {
	Data data;
	struct _node* left, * right;
	int height;
} Node;

// 노드 생성
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

// 노드의 높이 반환하는 함수
int get_height(Node* node) {
	if (node == NULL) {
		return 0;
	}
	return node->height;
}

// 노드의 높이 계산하는 함수 
void set_height(Node* node) {
	node->height = 1 + max(get_height(node->left), get_height(node->right));
}

// 왼쪽/오른쪽 서브트리의 높이 차이를 구하는 함수 
int diff_height(Node* node) {
	if (node == NULL) {
		return 0;
	}
	return get_height(node->left) - get_height(node->right);
}

// right subtree of right child -> single left rotation
Node* left_rotate(Node* node) {
	Node* right_child = node->right;
	
	//노드의 오른쪽에 오른쪽자식의 왼쪽 노드를 놓고, 오른쪽 자식의 왼쪽에 노드를 놓는다.
	node->right = right_child->left;
	right_child->left = node;
	
	// rotation 후 높이 재계산
	set_height(node);
	return right_child;
}

// left subtree of left child -> single right rotation
Node* right_rotate(Node* node) {
	Node* left_child = node->left;
	
	//노드의 왼쪽에 왼쪽자식의 오른쪽 노드를 놓고, 왼쪽 자식의 오른쪽에 노드를 놓는다. 
	node->left = left_child->right;
	left_child->right = node;
	
	// rotation 후 높이 재계산
	set_height(node);
	return left_child;
}

// 트리의 균형 유지하는 함수
Node* reblalacing(Node* node)
{
	int h = diff_height(node);
	if (h > 1) { // 왼쪽 서브트리의 높이가 더 크다 
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
	else if (h < -1) { // 오른쪽 서브트리의 높이가 더 크다
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

// 노드 삽입하는 함수
Node* insert_node(Node* node, Data data) {
	if (node == NULL) {
		node = new_node(data);
		return node;
	}
	if (data > node->data) { // data가 노드보다 크면 노드의 오른쪽에 삽입
		node->right = insert_node(node->right, data);
	}
	else if (data < node->data) { // data가 노드보다 작으면 노드의 왼쪽에 삽입
		node->left = insert_node(node->left, data);
	}
	else
		return node;

	// 삽입 후 트리의 균형 잡기
	node = reblalacing(node);
	return node;
}

// 트리 탐색하는 함수
Node* search_node(Node* node, Data data) {
	if (node == NULL) { // 값을 못 찾은 경우
		return NULL;
	}
	if (data == node->data) { // 값을 찾은 경우
		return node;
	}
	else if (data > node->data) { //값이 노드보다 큰 경우 오른쪽 서브트리 탐색
		search_node(node->right, data);
	}
	else { // 값이 노드보다 작은 경우 왼쪽 서브트리 탐색
		search_node(node->left, data);
	}
}


// 가장 왼쪽의 노드, 최소값을 가져오는 함수
Node* min_value_node(Node* node) {
	Node* current = node;
	while (current->left != NULL) {
		current = current->left;
	}
	return current;
}

// 노드 제거하는 함수
Node* delete_node(Node* node, Data data) {
	// base case  
	if (node == NULL)
		return node;

	// 삭제할 노드를 찾는다.    
	if (data < node->data) {
		node->left = delete_node(node->left, data);
	}

	else if (data > node->data) {
		node->right = delete_node(node->right, data);
	}
	// 삭제할 노드를 찾은 경우
	else {
		Node* temp;
		// 노드에 자식이 하나 있거나 없는 경우
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

		// 노드에 자식이 둘 있는 경우
		// 제거하고자 하는 키값보다 큰 값들 중 최소값을 갖는 노드를 찾음
		temp = min_value_node(node->right);
		// 찾은 노드의 키값을 제거하고자 하는 노드에 대체
		node->data = temp->data;
		// 대체 하였음으로 찾은 노드 제거
		node->right = delete_node(node->right, temp->data);
	}


	node = reblalacing(node);

	return node;
}

// avl 트리를 출력하는 함수 
void print_avl(Node* node, int level) {
	if (node != NULL) {
		// 가장 오른쪽 노드부터 방문 후 왼쪽 노드 방문
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