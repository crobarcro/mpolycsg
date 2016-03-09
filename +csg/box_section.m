classdef box_section < csg.polyhedron
    
    methods 
    
        function this = box_section (d, b, di, bi, len, is_centered)
        
            if nargin < 4
                is_centered = 1;
            end
            
            if (d <= di) || (b <= bi)
                error ('di an bi must be less than d and b');
            end
            
            this = this@csg.polyhedron ();
            
            this.makebox (b, d, len, is_centered);
            
            innerbore = csg.cube (bi, di, len+0.1*len, is_centered);
            
            if ~is_centered
                innerbore.translate ([0,0,-0.05*len]);
            end
            
            this.difference (innerbore);
            
        end
        
        % Operators
        function r = plus (a, b)
            % call parent class method
            r = plus@csg.polyhedron (a,b);
        end
        
        function r = minus (a, b)
            % call parent class method
            r = minus@csg.polyhedron (a,b);
        end
    
    end

end