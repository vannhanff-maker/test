#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
// #include <mylib.h>
using namespace std;

struct Sinhvien{
	int maso;
	char ho[51];
	char ten[11];
	float dtb;
	int hang;
};

struct node{
	Sinhvien sv;
	node *next;
};
typedef node* PTR;

// const int MAXLIST = 100

const int so_item = 16;
const int dong = 5;
const int cot = 40;
char thucdon [so_item][50] = {
	"1. Nhap danh sach sinh vien 				",
	"2. Liet ke danh sach 						",
	"3. Them ve dau danh sach 					",
	"4. Them sv o vi tri i						",
	"5. Xoa sinh vien theo ma so				",
	"6. Them 1 sv vao danh sach co thu tu		",
	"7. Loc SV trung theo ten					",
	"8. Sap xep dssv theo ma so tang dan		",
	"9. Save DSSV								",
	"10. Load DSSV				 				",
	"11. Xep hang theo DTB 						",
	"12. Xoa sinh vien theo ten					",
	"13. Dao nguoc danh sach					",
	"14. Copy danh sach							",
	"15. In DSSV tag dan theo ten (giu nguyen ds)",
	"16. Ket thuc chuong trinh					",
};

// them vao cuoi
int Insert_After(PTR p, Sinhvien x){
	PTR q;
	if(p == NULL) return 0;
	q = new node; // tao node moi
	q->sv = x;
	q->next = p->next; // = NULL
	p->next = q; // gan node moi vao 
	return 1;
}

// xoa dau
int Delete_First(PTR &First){
	PTR p;
	if(First == NULL) return 0;
	p = First; // nut can xoa la nut dau
	First = p->next;
	delete p;
	return 1;
}

int Menu (char td [so_item][50]){
	system("cls");   int chon =0;
	int i;
	for ( i=0; i< so_item ; i++){ 
		gotoxy(cot, dong +i);
		cout << td[i];
	}
	Nhaplai:
	  	gotoxy (cot, dong + so_item);
	  	cout << "Ban chon 1 so (1.."<< so_item << ") :    ";
	  	gotoxy (wherex()-4, wherey());
	  	cin >> chon;
	  	if (chon <1 || chon >so_item) goto Nhaplai;
	  	return chon;
}

void BaoLoi (char *s){
	int x=wherex() , y=wherey();
	gotoxy (10,24);
	cout <<s;
	Sleep(2000);
	gotoxy(10,24);
	clreol();
	gotoxy(x,y);
}

// tim kiem sinh vien theo ma so x
PTR Search(PTR First, int x){
	for(PTR p = First; p != NULL; p = p->next)
		if(p->sv.maso == x) return p;
	return NULL;
}

int NhapSV(PTR First , Sinhvien &sv){
 NhaplaiMaSV:
		cout << "\nMa so sinh vien (<=0 dung nhap):" ;
		cin >> sv.maso ;
		if (sv.maso <=0 ) return 0;
		if (Search (First, sv.maso)!= NULL)  {
			BaoLoi( "Ma so sinh vien bi trung. Ban nhap lai. ");
			goto NhaplaiMaSV;
		}
		fflush (stdin);
		cout << "Ho sinh vien  :" ; gets (sv.ho);
		fflush (stdin);
		cout << "Ten sinh vien :" ; gets (sv.ten);
		cout << "Diem TB :" ; cin >>sv.dtb;
		sv.hang =0;
		return 1;
}

// them vao dau
void Insert_First(PTR &First, Sinhvien &sv){
	PTR p = new node;
	p->sv = sv;
	p->next = First;
	First = p;
}

// them vao dau
void NhapSV_VeDau(PTR &First){
	system ("cls");
	PTR p; Sinhvien sv; int err;
	while(1){
		err = NhapSV(First, sv);
		if(err == 0) return;
		Insert_First(First, sv);
	}
}

// them cuoi
void Nhap_DSSV(PTR &First){
	system ("cls");
	PTR p, Last; SinhVien sv, int err;
	if(First != NULL)
		for(Last = First; Last->next != NULL; Last = Last->next); // tro den phan tu cuoi
	while(true){
		err = NhapSV(First, sv);
		if(err == 0) return;
		p = new node;
		p->sv = sv;
		p->next = NULL;
		if(First == NULL) First = p;
		else Last->next = p;
		Last = p;
	}
}

// Liet ke danh sach 
void LietKe(PTR First){
	int dem = 0;
	system("cls");
	cout << "MaSV		Ho				Ten		DTB		Hang\n";
	for(PTR p = First; p != NULL; p = p->next){
		cout << "%5d %-20s %-10s %.1f %5d\n", 
		p->sv.maso, p->sv.ho, p->sv.ten, p->sv.dtb, p->sv.hang;
		dem++;
	}
	cout << "\nSo luong Sv = " << dem;
	getch();
}

// dem 
int Reccount(PTR First){
	int dem = 0;
	for(PTR p = First; p != NULL; p = p->next)
		dem++;
	return dem;
}

// luu file
int SaveFile(PTR First, char *tenfile){
	FILE *f = fopen(tenfile, "wb");
	if(f == NULL) return 0;
	for(PTR p = First; p != NULL; p = p->next)
		fwrite(&p->sv, sizeof (Sinhvien), 1, f);
	fclose(f);
	return 1;
}

// them nut vao cuoi
void InsertLast(PTR &First, Sinhvien sv){
	PTR p = new node;
	p->sv = sv;
	p->next = NULL;
	if(First == NULL) First = p;
	else{
		PTR Last;
		for(Last = First; Last->next != NULL; Last = Last->next);
		Last->next = p;
	}
}

// mo file
int OpenFile(PTR &First, char *tenfile){
	PTR p; Sinhvien sv;
	FILE *f = fopen(tenfile, "rb");
	if(f == NULL) return 0; // mo file khong duoc
	while(First != NULL){
		p = First;
		First = p->next;
		delete p;
	}
	while(fread (&sv, sizeof (Sinhvien), 1, f) == 1)
		InsertLast(First, sv);
	fclose(f);
	return 1;
}

bool XacNhan (char *S) {
	cout << S;  char kt;
	do {
	   kt = toupper(getch());	
	} while (kt!='Y' && kt !='N');
	cout <<kt  ;
	return kt=='Y';
}

// them phan tu theo ma
void Insert_Order(PTR &First, Sinhvien x){
	PTR p, t, s; // t la nut truoc, s la nut sau
	p = new node;
	p->sv = x;
	for(s = First; s != NULL && s->sv.maso < x.maso; t=s, s = s->next);
	// them nut vao dau danh sach lien ket
	if(s == First){
		p->next = First;
		First = p;
	}
	// them nut p vao truoc nut s sau nut t
	else{
		p->next = s;
		t->next = p;
	}
}

// xoa phan tu sau nut p
int Delete_after(PTR p){
	PTR q;
	if((p == NULL) || (p->next == NULL)) return 0;
	q = p->next;
	p->next = q->next;
	delete q;
	return 1;
}

// them sinh vien o vi tri thu i
int Them_SV_Taivitri(PTR &First, Sinhvien sv, int i){
	// kiem tra vi tri i co hop le
	sl = Reccount(First);
	if(i < 0 || i > sl) return 0;
	
	// neu chen dau
	if(i == 0){
		Insert_First(First, sv);
		return 1;
	}
	
	// tim node dung truoc i
	PTR p = First;
	for(int k = 0; k < i - 1; k++)
		p = p->next;
	// chen sau p
	return Insert_After(p, sv);
}

// xoa sinh vien theo ma
int Xoa_SV_theo_Masv (PTR &First, int maso){
	if(First == NULL) return 0;
	
	PTR p = First;
	PTR t = NULL;
	
	// tim sinh vien can xoa
	while(p != NULL && p->sv.maso != maso){
		t = p;
		p = p->next;
	}
	
	if(p == NULL) return 0; // khong tim thay
	
	// neu xoa dau danh sach
	if(t == NULL)
		First = p->next;
	else
		t->next = p->next;
	delete p;
	return 1;
}

// loc sinh vien trung theo ten 
PTR filter(PTR First){
	PTR head = NULL; // danh sach ket qua
	
	for(PTR p = First; p != NULL; p = p->next){
		int dem = 0;
		
		// dem xem co bao nhiu nguoi cung ten
		for(PTR q = First; q != NULL; q = q->next){
			if(strcmp(p->sv.ten, q->sv.ten) == 0)
				dem++;
		}
		
		// neu ten xuat hien >= 2 lan thi dua vao ds
		if(dem >= 2){
			InsertLast(head, p->sv);
		}
	}
	return head;
}

// sap xep theo ten nho den lon tu a -> z
void SapXepTangTheoTenVaHo(PTR First){
	for(PTR p = First; p != NULL; p = p->next){
		for(PTR q = p->next; q != NULL; q = q->next){
			// neu ten lon hon || trung ten nhung ho lon hon
			if((strcmp(p->sv.ten, q->sv.ten) > 0) ||
			((stcmp(p->sv.ten, q->sv.ten) == 0) && (strcmp(p->sv.ho, q->sv.ho) > 0))){
				// hoa vi du lieu
				Sinhvien tmp = p->sv;
				p->sv = q->sv;
				q->sv = tmp;
			}
		}
	}
}

// xep hang theo dtb
// dieu kien la da sap xep theo ho ten roi
void xepHang(PTR First){
	if(First == NULL){
		BaoLoi("Danh sach rong.")
		return;
	}
	
	// tinh hang cho tung sinh vien
	for(PTR p = First; p != NULL; p = p->next){
		int rank = 1;
		for(PTR q = First; q != NULL; q = q->next){
			if(q->sv.dtb > p->sv.dtb)
				rank++;
		}
		p->sv.hang = rank;
	}
	getch();
}

// xoa sinh vien theo ten
int XoatheotenSV(PTR &First, char tensv[]){
	if(First == NULL) return 0;
	
	int dem = 0;
	
	// xoa dau neu trung
	while(First != NULL && strcmp(First->sv.ten, tensv) == 0)
		dem += Delete_First(First);
		
	// xoa cac phan tu sau
	PTR p = First;
	while(p != NULL && p->next != NULL){
		if(strcmp(p->next->sv.ten, tensv) == 0){
			dem += Delete_after(p);
		}
		else{
			p = p->next;
		}
	}
	return dem; // tra ve so sinh vien da xoa
}

// dao nguoc danh sach
void reverseLinkedList(PTR &First){
	PTR cur = First;
	PTR prev = NULL;
	PTR next = NULL;
		
	while(cur != NULL){
		next = cur->next; // luu not ke tiep
		cur->next = prev; // dao chieu lien ket
		prev = cur; // doi prev len
		cur = next; // doi cur len
	}	
	First = prev; // cap nhat lai
}

// sao chep danh sach
PTR SaoChepDanhSach(PTR First){
	PTR First2 = NULL;
	for(PTR p = First; p != NULL; p = p->next)
		InsertLast(First2, p->sv);
	return First2;
}

// in damj sach tang theo ten
void InDSSVTangTheoTen(PTR First){
	if(First == NULL){
		BaoLoi("Danh sah rong");
		return;
	}
	
	// copy danh sach
	PTR First2 = SaoChepDanhSach(First);
	
	// sap xep danh sach tang theo ten
	SapXepTangTheoTenVaHo(First2);
	
	// in danh sach
	LietKe(First2);
}

// xoa danh sach
void clearlist(PRT &First){
	PTR p;
	while(First != NULL){
		p = First;
		First = First->next;
		delete p;
	}
}

enum ActionType {
	THEM, XOA, SUA
};

struct Action{
	ActionType type;
	Sinhvien oldData; // du lieu cu cho xoa, sua
	Sinhvien newData; // du lieu moi cho sua
};

stack<Action> undoStack;

void Undo(PTR &First){
	if(undoStack.empty()){ // neu undoStack rong
		cout << "Khong co thao tac de Undo.\n";
		return;
	}
	
	Action a = undoStack.top();
	undoStack.pop();
	
	if(a.type == THEM){
		// undo them = xoa
		Xoa_SV_theo_Masv(First, a.oldData.maso);
	}
	else if(a.type == XOA){ // vidu xoa vi tri cuoi
		// Undo xoa = them lai
		InsertLast(First, a.oldData);
	}
	else if(a.type == SUA){
		// Undo sua = phuc hoi du lieu cu
		PTR p = Search(First, a.oldData.maso);
		if(p != NULL)
			p->sv = a.oldData;
	}
	cout << "Undo thanh cong.";
}

int main(){
	char filename[80] = "D:\\DSSV_DTB.TXT";
	int x;
	char tensv[16];
	int chon; // chon menu
	PTR head;
	PTR First = NULL;
	int so;
	PTR First1 = NULL, First2 = NULL;
	int i;
	Sinhvien sv;
	int maso; 

	do{
	    chon = Menu (thucdon);
	    switch (chon ){
		    case 1: Nhap_DSSV(First) ; break;
		    
		    case 2: LietKe(First) ; break;
		    
		    case 3: {
		    	system ("cls");
				printf("So luong sinh vien =%d", Reccount(First));
				getch();
				system("cls");
				if(NhapSV(First, sv) == 0) break;
				Insert_First(First, sv);
				BaoLoi("Da them sinh vien vao dau danh sach thanh cong.");
				
				Action a;
				a.type = THEM;
				a.oldData = sv;
				undoStack.push(a);
				break;
		    }
		    
			case 4:{
				system ("cls");
				cout << "Nhap vi tri thu i can them : "; cin >> i;			
				if(NhapSV(First, sv) == 0) break;			
				int err = Them_SV_Taivitri(First, sv, i);
				if(err == 1)
					BaoLoi("Them thanh cong.");
				else
					BaoLoi("Vi tri khong hop le.");
				break; 
		    }
		    
		    case 5: {
		    	system ("cls");
		        cout << "Ma so SV muon xoa :" ; cin >> maso;
		        int err= Xoa_SV_theo_Masv (First, maso);
		        if (err==1)
		          BaoLoi("Da xoa sinh vien co ma so trong DSSV.");
		        else BaoLoi ("Ma so SV khong co trong danh sach."); 
		        
		        Action a;
			    a.type = XOA;
			    a.oldData = p->sv;
			    undoStack.push(a);
			    
				break;
			}
			
		    case 6: {
		    	system ("cls");
		    	if(NhapSV(First, sv) == 0) break;
		    	Insert_Order(First, sv);
		    	BaoLoi("Da them sinh vien vao danh sach co thu tu.");
				break;
			}  
			  
		    case 7: {
		    	system ("cls");
		        head = filter(First);
		        if(head == NULL)
		        	BaoLoi("Khong co sinh vien trung ten.")
		        else{
		        	for (PTR i=head;i!=NULL;i=i->next) 
			    		printf ("%-10s ",  i->sv.ten );
			    	getch();
				}	
		        break;
		    }
		    
		    case 8: {
		    	system ("cls");
		    	SapXepTangTheoTenVaHo(First); 
		        BaoLoi("Da sap xep xong DSSV theo ten+ho tang dan");
				break;
			}
			
		    case 9: {
		    	system ("cls");
		        if (SaveFile(First, filename)==0) cout << "Loi mo file de ghi ";
		        else cout << "Da ghi xong danh sasch sinh vien vao file.";
		        Sleep(2000);
				break;
			}
		
		    case 10:{
		    	system ("cls");
		        if (OpenFile(First, filename)==0) cout << "Loi mo file de doc ";
		        else cout << "Da doc xong danh sasch sinh vien tu file.";
		        Sleep(2000);
				break;
			} 
		
			case 11: {
				xepHang(First);
				BaoLoi("Da xep hang cho sinh vien theo dtb thanh cong.")
			    break;  
			}
			
			case 12: { 
		        system ("cls");
		        cout << "Ten SV muon xoa :" ; fflush(stdin); gets (tensv);
		        cout << "So SV da xoa =" << XoatheotenSV(First,tensv);
				getch(); 
		        break;	
		    }
			
			case 13: {
				system ("cls");
		        reverseLinkedList(First);
		        cout << "Da dao nguoc danh sach xong"; 
				getch(); 
		        break;
		    }
		
			case 14: {
				system ("cls");
			    First2 = SaoChepDanhSach(First);
			    LietKe(First2);
			    getch();
		        break;   
			}  
			   
			case 15: {
				system ("cls");
		    	InDSSVTangTheoTen(First);
		    	getch();
		    	break;
		    }
		    
		    /*
		    case 17:{
		    	Undo(First);
		    	getch();
		    	break;
		    }
		    */
		
		    case so_item : {
				// clearlist(First); clearlist(First2); 
			    return 0; 
		    }
	    }
	}while (1);

	return 0;
}
