%gcode parsing


lin = textread('gcodeTest.txt','%d', 'delimiter',' ','whitespace','');
i = 1;
while i < numel(lin)
    
    disp(lin(i));
    comm = lin(i);
    if (lin(i) == 1)
        %move command
        
        tool = lin(i+1);
        xpos = lin(i+2)/100;
        ypos = lin(i+3)/100;
        ang = lin(i+4);
        
        
        %send data to motors
        
        
        i = i + 5;
       
    elseif (lin(i) == 2)
        %draw command
        xpos = lin(i+1)/100;
        ypos = lin(i+2)/100;
        ang = lin(i+3);
        
        %send data to motors
        
        
        
        i = i + 4;
        
        
    else 
        fprintf('Error parsing gcode file');
    end
         
     
    
end

