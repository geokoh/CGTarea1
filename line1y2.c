void line(int x0,int y0,int x1,int y1){

	float m,b,y;
	int i, roundedY;
	int delta_x, delta_y;

	delta_x = (x1-x0);
	delta_y = (y1-y0);
	if (delta_x==0) {
	    if(y1>y0){
	        for (i=y0; i<= y1; i++ ) {
	            plot(x0, i);
	        }
	    } else {
	        for (i=y1; i<= y0; i++ ) {
	            plot(x0, i);
	        }
	    }
	} else {
	    m= (((float)delta_y)/((float)delta_x));
    	b = y0-m*x0;
    
    	if(abs(delta_x)>abs(delta_y)){
    		if(x1>x0) {
    		    for(i=x0; i <= x1; i++) {
    			    y = m*i + b;
    			    roundedY = round(y);
    			    plot(i, roundedY);
    		    }
    		} else {
    		    for(i=x1; i <= x0; i++) {
    			    y = m*i + b;
    			    roundedY = round(y);
    			    plot(i, roundedY);
    		    }
    		}
    	}else{
    	    if(y1>y0) {
        		for(i=y0; i <= y1; i++){
        			y = (i - b)/m;
        			roundedY = round(y);
        			plot(roundedY, i);
        		}
    	    } else {
    	        for(i=y1; i <= y0; i++){
        			y = (i - b)/m;
        			roundedY = round(y);
        			plot(roundedY, i);
        		}
    	    }
    	}
	}
}

void line2(int x0,int y0,int x1,int y1){

	float m, y;
	int i, roundedY;
	int delta_x, delta_y;

	delta_x = (x1-x0);
	if (delta_x==0) {
	    if(y1>y0){
	        for (i=y0; i<= y1; i++ ) {
	            plot(x0, i);
	        }
	    } else {
	        for (i=y1; i<= y0; i++ ) {
	            plot(x0, i);
	        }
	    }
	} else {
	    delta_y = (y1-y0);
    	m= (((float)delta_y)/((float)delta_x));
    	
    
    	if(abs(delta_x)>abs(delta_y)){
    	    if(x1>x0) {
    	        y = y0;
        	    for(i=x0; i<=x1; i++){
        		    roundedY = round(y);
        			plot(i, roundedY);
        			y +=m;
        		}
    	    } else {
    	        y = y1;
    	        for(i=x1; i<=x0; i++){
        		    roundedY = round(y);
        			plot(i, roundedY);
        			y +=m;
        		}
    	    }
    		
    	}else{
    	    if(y1>y0) {
    	        y = x0;
    	        for(i=y0; i<=y1; i++){
        		    roundedY = round(y);
        			plot(roundedY, i);
        			y +=(1/m);
        		}
    	    } else {
    	        y = x1;
    	        for(i=y1; i<=y0; i++){
        		    roundedY = round(y);
        			plot(roundedY, i);
        			y +=(1/m);
        		}
    	    }
    		
    	}
	}
	

}