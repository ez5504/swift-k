//----------------------------------------------------------------------
//This code is developed as part of the Java CoG Kit project
//The terms of the license can be found at http://www.cogkit.org/license
//This message may not be removed or altered.
//----------------------------------------------------------------------

/*
 * Created on Dec 19, 2012
 */
package org.globus.cog.karajan.compiled.nodes;

import k.rt.ExecutionException;
import k.rt.Stack;
import k.thr.LWThread;
import k.thr.Yield;

import org.globus.cog.karajan.analyzer.CompilationException;
import org.globus.cog.karajan.analyzer.ContainerScope;
import org.globus.cog.karajan.analyzer.Scope;
import org.globus.cog.karajan.parser.WrapperNode;


public abstract class FramedInternalFunction extends InternalFunction {
	private int varCount;

	public Node compile(WrapperNode w, Scope scope) throws CompilationException {
		ContainerScope cs = new ContainerScope(w, scope);
		Integer l = (Integer) WrapperNode.getTreeProperty(WrapperNode.LINE, w);
		if (l != null) {
			setLine(l);
		}
		setType(w.getNodeType());
		Node fn = compileChildren(w, cs);
		varCount = cs.size();
		return fn;
	}
	
	protected void setVarCount(int count) {
		this.varCount = count;
	}
	
	protected void enter(Stack stack) {
		stack.enter(this, varCount);
	}
	
	protected void leave(Stack stack) {
		stack.leave();
	}
	
	@Override
	public void run(LWThread thr) {
		int ec = childCount();
        int i = thr.checkSliceAndPopState();
        Stack stack = thr.getStack();
        try {
	        switch (i) {
	        	case 0:
	        		stack.enter(this, varCount);
	        		initializeArgs(stack);
	        		i++;
	        	default:
			            for (; i <= ec; i++) {
			            	runChild(i - 1, thr);
			            }
			            i = Integer.MAX_VALUE;
	        	case Integer.MAX_VALUE:
			            try {
			            	runBody(thr);
			            	stack.leave();
			            }
			            catch (RuntimeException e) {
			            	throw new ExecutionException(this, e);
			            }
		    }
        }
        catch (Yield y) {
            y.getState().push(i);
            throw y;
        }
	}
}
