/* Copyright (c) 2002-2003 krzYszcz and others.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include "m_pd.h"

typedef struct _onebang
{
    t_object  x_ob;
    int       x_isopen;
    t_outlet * x_r_bng;
} t_onebang;

static t_class *onebang_class;

static void onebang_bang(t_onebang *x)
{
    if (x->x_isopen)
    {
        outlet_bang(((t_object *)x)->ob_outlet);
        x->x_isopen = 0;
    }
    else
        outlet_bang(x->x_r_bng);
}

static void onebang_float(t_onebang *x, t_float f)
{
    onebang_bang(x);
}


static void onebang_symbol(t_onebang *x, t_symbol *s)
{
    onebang_bang(x);
}


static void onebang_list(t_onebang *x, t_symbol *s, int ac, t_atom *av)
{
    onebang_bang(x);
}

static void onebang_anything(t_onebang *x, t_symbol *s, int ac, t_atom *av)
{
    onebang_bang(x);
}

static void onebang_bang1(t_onebang *x)
{
    x->x_isopen = 1;
}

static void onebang_stop(t_onebang *x)
{
    x->x_isopen = 0;
}

static void *onebang_new(t_floatarg f)
{
    t_onebang *x = (t_onebang *)pd_new(onebang_class);
    x->x_isopen = ((int)f != 0);  /* CHECKED */
    inlet_new((t_object *)x, (t_pd *)x, &s_bang, gensym("bang1")); // change to anything
    outlet_new((t_object *)x, &s_bang);
    x->x_r_bng = outlet_new((t_object *)x, &s_bang);
    return (x);
}

void onebang_setup(void)
{
    onebang_class = class_new(gensym("onebang"),
			      (t_newmethod)onebang_new, 0,
			      sizeof(t_onebang), 0, A_DEFFLOAT, 0);
    class_addbang(onebang_class, onebang_bang);
    class_addmethod(onebang_class, (t_method)onebang_bang1,
		    gensym("bang1"), 0);
    class_addmethod(onebang_class, (t_method)onebang_stop,
                    gensym("stop"), 0);
    class_addfloat(onebang_class, onebang_float);
    class_addsymbol(onebang_class, onebang_symbol);
    class_addlist(onebang_class, onebang_list);
    class_addanything(onebang_class, onebang_anything);
}
