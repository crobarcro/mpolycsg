classdef cube < csg.polyhedron
    
    methods 
    
        function this = cube (x, y, z, is_centered)
        
            if nargin < 4
                is_centered = 1;
            end
            
            this = this@csg.polyhedron ();
            
            this.makebox (x, y, z, is_centered);
            
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