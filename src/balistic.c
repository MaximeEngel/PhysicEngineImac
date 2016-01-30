/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Université Paris-Est-Marne-la-Vallée                          =*/
/*= Balistic launch : analytic and step-by-step solutions         =*/
/*=================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <g2x.h>

#define g 10.0

/* pixel window dimensions          */
uint   pixwidth=1340,pixheight=680;
/* floating-point window dimensions */
double xmin=-5.,ymin=-5.,xmax=+201.,ymax=+101.;

/* 'Physical' parameters                                                     */
double m=1.0;           /* ball mass                                         */
double r=2.5;           /* ball ray (used for collision detection)           */
double kc=0.75,zc=0.95; /* 'ground' parameters for inverse kinetic collision */
double za=0.0;          /* air 'viscosity' (elementary kinetic damping)      */

/* initial position and speed */
G2Xpoint  P0=(G2Xpoint) {0.,24.};
G2Xvector V0=(G2Xvector){8.,32.};
/* current positions */
G2Xpoint  Pe,Pi,Pl;
/* speed, acceleration, gravity */
G2Xvector Ve,Vi,Vl,Ae,Ai,Al,G;
/* sampling rate for images */
double dt=0.1;
/* simulation time step */
double h =0.001;
/* ratio FAFF=(int)(dt/h) */
int    FAFF;


/*======================================================================*/
/*= Computes an draws analytic solution for initial conditions (p0,vO) =*/
/*======================================================================*/
void Analytic(G2Xpoint* p0, G2Xvector* v0, double *t0)
{
	G2Xpoint  p,q,v;	
	double t  = *t0;
	double ti;
	g2x_Circle(p0->x,p0->y,r,G2Xwc,1);
	q=*p0;
	do
	{
		if (G2Xzero(za)) /* collision detection and treatment */
		{ 
			p.x = p0->x + v0->x*t;
			p.y = p0->y + v0->y*t- 0.5*g*t*t;		
			v.x = v0->x;
			v.y = v0->y - g*t;
		}
		else /* free motion */
		{
			/* shortcuts */
			double w=m/za,gw=g*w;
			double e   = exp(-t/w);
			/* setup position */
			p.x = p0->x + w*(   v0->x)*(1.-e)       ;
			p.y = p0->y + w*(gw+v0->y)*(1.-e) - gw*t;		
			/* setup speed */
			v.x = v0->x*e;
			v.y = (gw+v0->y)*e-gw;
		}
		g2x_Line(q.x,q.y,p.x,p.y,G2Xwa,1);
		g2x_Plot(p.x,p.y,G2Xwb,3);
		q=p;
		t+=dt;
	} while (p.y+v.y*dt>r);
  p0->x = p.x+v.x*dt;
	p0->y = r;
	g2x_Line(q.x,q.y,p0->x,p0->y,G2Xwa,1);

	/* initial speed after collision -- inverse kinematics */
	v0->x =  zc*v.x;
	v0->y = -kc*v.y;
}

/*=================================================================*/
/*= Analytic solution is drawn and saved as backgroung image      =*/
/*=================================================================*/
void Background(void)
{
	double u=0;
	G2Xpoint  p=P0;	
	G2Xvector v=V0;		
	g2x_Axes();
	do 
		Analytic(&p,&v,&u);
	while (!G2Xzero(v.x) && p.x<xmax);
}


/*=================================================================*/
/*= sic.                                                          =*/
/*=================================================================*/
void reset(void)
{	
	Pe=Pi=Pl=P0;
	Ve=Vi=Vl=V0;
	Ae.x=Ai.x=Al.x=G.x-(za/m)*V0.x;
	Ae.y=Ai.y=Al.y=G.y-(za/m)*V0.y;
}

/*=================================================================*/
/*= Initialize                                                    =*/
/*=================================================================*/
void Init(void)
{
	FAFF=(int)(dt/h);
	G.x=0;
	G.y=-g;
	reset();
	/* flush a new view (draw) at each 'FAFF' simulation step */
  g2x_SetRefreshFreq(FAFF);
}


/*=================================================================*/
/*= step-by-step function called by the simulation loop           =*/
/*=================================================================*/
/*= Explicit method =*/
void Anim_Explicit(void)
{  
	/* 1 - set up ball position */
	Pe.x += h*Ve.x;
	Pe.y += h*Ve.y;
	/* 2 - set up ball velocity */
	Ve.x += h*Ae.x; 
	Ve.y += h*Ae.y; 
  /* 3 - computes the resulting force applied to the ball       */
	Ae.x=G.x-(za/m)*Ve.x;
	Ae.y=G.y-(za/m)*Ve.y;
  /* collision                                              */
	if (Pe.y<=r) 	/* detection                                */
	{
		Ve.x *=  zc; /*                                          */
		Ve.y *= -kc; /* treatment : inverse kinetics             */
		Pe.y  = r;   /*                                          */
	}
}

/*= Implicit method =*/
void Anim_Implicit(void)
{  
	/* 1 - set up ball velocity */
	double w = m/(m+za*h);
	Vi.x = (Vi.x + G.x*h) * w; 
	Vi.y = (Vi.y + G.y*h) * w; 
	/* 2 - set up ball position */
	Pi.x += h*Vi.x;
	Pi.y += h*Vi.y;
  /* 3 - computes the resulting force applied to the ball       */
	Ai.x=G.x-(za/m)*Vi.x;
	Ai.y=G.y-(za/m)*Vi.y;
  /* collision                                              */
	if (Pi.y<=r) 	/* detection                                */
	{
		Vi.x *=  zc; /*                                          */
		Vi.y *= -kc; /* treatment : inverse kinetics             */
		Pi.y  = r;   /*                                          */
	}
}


/*= Hybrid method =*/
void Anim_LeapFrog(void)
{  
	/* 1 - set up ball velocity */
	Vl.x += h*Al.x; 
	Vl.y += h*Al.y; 
	/* 2 - set up ball position */
	Pl.x += h*Vl.x;
	Pl.y += h*Vl.y;
  /* 3 - computes the resulting force applied to the ball       */
	Al.x=G.x-(za/m)*Vl.x;
	Al.y=G.y-(za/m)*Vl.y;
  /* collision                                              */
	if (Pl.y<=r) 	/* detection                                */
	{
		Vl.x *=  zc; /*                                          */
		Vl.y *= -kc; /* treatment : inverse kinetics             */
		Pl.y  = r;   /*                                          */
	}
}

void Anim(void)
{
	/* flush a new view (draw) at each 'FAFF' simulation step */
	FAFF=(int)(dt/h);
  g2x_SetRefreshFreq(FAFF);

  Anim_Explicit();
  Anim_Implicit();
  Anim_LeapFrog();

	/* the ball leaves the window, back to initial conditions */
	if (Pe.x>=0.95*xmax || Pi.x>=0.95*xmax || Pl.x>=0.95*xmax) reset();

}

/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
void Draw(void)
{
	Background(); /* call the background image */
	g2x_FillCircle(Pe.x,Pe.y,r,G2Xb);
	g2x_FillCircle(Pi.x,Pi.y,r,G2Xg);
	g2x_FillCircle(Pl.x,Pl.y,r,G2Xr);
	g2x_StaticPrint(10,10,G2Xk,'l',"dt=%.1e  h=%.1e  FAFF=%d",dt,h,FAFF);
	g2x_StaticPrint(100,600,G2Xb,'l',"Explicit");
	g2x_StaticPrint(180,600,G2Xg,'l',"Implicit");
	g2x_StaticPrint(250,600,G2Xr,'l',"LeapFrog");
}

/*===========================================================================*/
/*= Cleaning function                                                       =*/
/*===========================================================================*/
void Quit(void)
{
  /* nothing to do here */
}

/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
int main(int argc, char* argv[])
{	  
  /* window statement */
  g2x_InitWindow("Balistic",pixwidth,pixheight);
  g2x_SetWindowCoord(xmin,ymin,xmax,ymax);
  /* dialog functions */
  g2x_SetInitFunction(Init);
	g2x_SetDrawFunction(Draw);  
	g2x_SetAnimFunction(Anim);  
  g2x_SetExitFunction(Quit);
  /* scrollbars and buttons */
	g2x_CreateScrollv_d("kc",&kc,0.,1.,.5,"kc");
	g2x_CreateScrollv_d("zc",&zc,0.,1.,.5,"zc");
	g2x_CreateScrollv_d("za",&za,0.,1.,2.,"za");
	g2x_CreateScrollh_d("dt",&dt,0.01,0.5,1.,"dt");
	g2x_CreateScrollh_d(" h",&h ,0.00001,1.,1.,"h");
	g2x_CreatePopUp("reset",reset,"reset");

  /* simulation loop -- passed to glutMainLoop() */
  return g2x_MainStart();
}
