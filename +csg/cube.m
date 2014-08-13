classdef cube < csg.polyhedron
    
    methods 
    
        function this = cube (x, y, z, is_centered)
        
            if nargin < 4
                is_centered = 1;
            end
            
            this = this@csg.polyhedron ();
            
            this.makebox (x, y, z, is_centered);
            
        end
    
    end


end