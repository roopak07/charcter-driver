#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
void display();
void add();
struct node
{
 int data;
 struct node *next;
};
struct node *root = NULL;

void main()
{
 int ch;
 while(1)
 {
  printf("************__List__*************\n");
  printf("1.Add\n");
  printf("2.display\n");
  printf("9.exit\n");
  printf("Enter your choise:");
  scanf("%d",&ch);
  switch(ch)
  {
	case 1: add();
			break;
	case 2:	display();
			break;
	case 9:exit(0);
	default : printf("Enter a valid choise \n");
  }
 }
}

/***************** Add function*****************/
void add()
{
 struct node *temp;
 temp = (struct node*)malloc(sizeof(struct node));
 printf("Enter data : ");
 scanf("%d",&temp->data);
 temp->next = NULL;
 if(root == NULL)
 {
	 root = temp;
	 printf("%d --> %d | %d \n",root,temp->data,temp->next);
 }
 else
 {
	 struct node *pos;
	 pos = root;
	 while( pos->next != NULL)
	 {
		 pos = pos -> next;
	 }
	 pos->next = temp;
	 printf("%d --> %d | %d \n",pos -> next,temp->data,temp->next);
 }
}

/***************** display function*****************/
void display()
{
	struct node *pos;
	pos = root;
	printf("%d",root);
	while(pos!= NULL)
	{
		printf(" --> %d | %d",pos -> data,pos -> next);
		pos = pos ->next;
	}
	printf("\n");
}