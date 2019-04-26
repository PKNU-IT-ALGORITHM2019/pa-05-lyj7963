#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 2200
#define numofowrd 200000

int Num = 0;
int check = 0;

typedef struct Dict {
	char *word;
	char *class_w;
	char *meaning;
	struct Dict* parent;
	struct Dict* left;
	struct Dict* right;
}Dict;

Dict* root = NULL;
Dict* insertnode(Dict* root, char *word, char *class_w, char *meaning, Dict* parent);
Dict* search(Dict* root, char* word);
void deletenode(Dict* root, char* word);
void inorder(Dict* node);
void read(char fileword[]);
void print(Dict* node);
void deleteall(char* filename, Dict* root);

Dict* search(Dict* root, char* word)
{
	Dict *r = root;
	Dict *r2 = root;
	while (strcmp(root->word, word) != 0)
	{
		if(strcmp(word, root->word) == 0) { return root; }
		else if (strcmp(word, root->word) < 0) {
			r = root;
			root = root->left;
		}
		else if (strcmp(word, root->word) > 0) {
			r2 = root;
			root = root->right;
		}

		if (root == NULL) {
			return NULL;
		}
	}
	return root;
}
Dict* insertnode(Dict* root, char *word, char *class_w, char *meaning, Dict* parent) 
	{
	Dict* n;
	if (root == NULL) {
		n = (Dict*)malloc(sizeof(Dict));
		n->word = strdup(word);
		n->class_w = strdup(class_w);
		n->meaning = strdup(meaning);
		n->left = NULL;
		n->right = NULL;
		n->parent = parent;
		Num++;
		return n;
	}
	else if (strcmp(word, root->word) < 0)
		root->left = insertnode(root->left, word, class_w, meaning, root);
	else if (strcmp(word, root->word) >= 0)
		root->right = insertnode(root->right, word, class_w, meaning, root);
	return root;
}
void read(char fileword[]) {
	char buf1[MAX];
	char buf2[MAX];

	char *wd, *cl, *mn;
	FILE* fp = fopen("shuffled_dict.txt", "r");

	if (fp == NULL)
		puts("error : No File\n");
	else {
		while (fgets(buf1, sizeof(buf1), fp) != NULL) {
			int j = 0;
			for (int i = 0; buf1[i] != '\0'; i++) {
				buf2[j] = buf1[i];
				j++;
			}
			buf2[j] = '\0';
			wd = strdup(strtok(buf2, "("));
			cl = strdup(strtok(NULL, ")"));
			mn = strdup(strtok(NULL, ""));
			wd[strlen(wd) - 1] = '\0';
			if (root == NULL) {
				root = insertnode(root, wd, cl, mn, root);
			}
			else {
				insertnode(root, wd, cl, mn, root);
			}
		}
	}
	fclose(fp);
}
void deletenode(Dict* root, char* word) {
	Dict* del = NULL;
	Dict* p = NULL;

	Dict* max_left = NULL;
	Dict* p_max_left = NULL;

	while (1) {
		if (root == NULL) {
			if (check == 1) return;
			printf("Not found.\n");
			return;
		}
		else if (root->left != NULL && strcmp(root->left->word, word) == 0) {
			del = root->left;
			p = root;
			break;
		}
		else if (root->right != NULL && strcmp(root->right->word, word) == 0) {
			del = root->right;
			p = root;
			break;
		}
		if (strcmp(word, root->word)<0)
			root = root->left;
		else if (strcmp(word, root->word)>0)
			root = root->right;
	}
	if (del->right == NULL && del->left == NULL) {
		if (p->right != NULL && p->right == del)
			p->right = NULL;
		else
			p->left = NULL;
	}
	else if ((del->right == NULL && del->left != NULL) || (del->right != NULL && del->left == NULL)) {
		if (del->right != NULL) {
			if (p->right == del)
				p->right = del->right;
			else
				p->left = del->right;
		}
		else {
			if (p->right == del)
				p->right = del->left;
			else
				p->left = del->left;
		}
	}
	else {
		max_left = del->left;
		p_max_left = del;

		while (max_left->right != NULL) {
			p_max_left = max_left;
			max_left = max_left->right;
		}
		if (del->left == max_left) {
			max_left->right = del->right;
		}
		else {
			p_max_left->right = NULL;
			max_left->left = del->left;
			max_left->right = del->right;
		}
		if (p->right == del)
			p->right = max_left;
		else
			p->left = max_left;
	}

	free(del);
	Num--;
	if (check == 1) return;
	printf("complete.\n");
}
void inorder(Dict* node)
{
	if (node->left)	inorder(node->left);
	print(node);
	if (node->right)	inorder(node->right);
}
void print(Dict* node) {
	if (node != NULL)
		printf("%s\nclass : %s\nmeaning : %s\n", node->word, node->class_w, node->meaning);
}
void deleteall(char* filename, Dict* root)
{
	char buf1[MAX];
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
		puts("error : No File\n");
	else {
		while (fgets(buf1, sizeof(buf1), fp) != NULL) {
			buf1[strlen(buf1) - 1] = '\0';
			deletenode(root, buf1);
		}
	}
}
void add()
{
	char wd[MAX], cl[MAX], mn[MAX];
	printf("word: ");
	gets_s(wd);
	printf("class: ");
	gets_s(cl);
	printf("meaning: ");
	gets_s(mn);
	insertnode(root, wd, cl, mn, root);
}
int main()
{
	Dict *s;
	char command[100];
	char *token;
	char tok[] = "";
	while (1)
	{
		printf("$ ");
		gets_s(command);
		token = strdup(strtok(NULL, tok));
		if (strcmp(token, "read") == 0) 
		{
			token = strdup(strtok(NULL, tok));
			read(token);
		}
		else if (strcmp(token, "size") == 0) 
		{
			printf("%d \n", Num);
		}
		else if (strcmp(token, "find") == 0) 
		{
			token = strdup(strtok(NULL, tok));
			s = search(root, token);
			if (s != NULL) {
				print(s);
			}
			else printf("Not found. \n");
		}
		else if (strcmp(token, "add") == 0) 
		{
			add();
		}
		else if (strcmp(token, "printall") == 0)
		{
			inorder(root);
		}
		else if (strcmp(token, "Deleteall") == 0)
		{
			check = 1;
			token = strdup(strtok(NULL, tok));
			deleteall(token, root);
			check = 0;
		}
		else if (strcmp(token, "exit") == 0)
		{
			break;
		}
		else printf("Wrong input.");
	}
}

