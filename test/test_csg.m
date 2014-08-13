


p = csg.polyhedron;
p.makebox(1,1,1);

n = p.num_faces ();

for face_id = 0:n-1

    facecoords = [];
    
    verts = p.get_face_vertices (face_id)
    
    for indii = 1:numel(verts)
    
        facecoords = [ facecoords; p.get_vertex(verts(indii)) ]
    
    end
   

end


%% 

p = csg.polyhedron;
p.makesphere(1,1);
p.render ()


%%

p = csg.polyhedron; 
p.makebox (1,1,1,0); 
p2 = csg.polyhedron; 
p2.makebox (0.5,0.5,0.5);

p.union (p2)

p.render ()


%%

p = csg.polyhedron; 
p.makebox (1,1,1,0); 
p2 = csg.polyhedron; 
p2.makebox (0.5,0.5,0.5);

p3 = p + p2;

p3.render ()

%%

p = csg.polyhedron; 
p.makebox (1,1,1,0); 
p2 = csg.polyhedron; 
p2.makebox (0.5,0.5,0.5);

p.difference (p2)

p.render ()


%%

p = csg.polyhedron; 
p.makebox (1,1,1,0); 
p2 = csg.polyhedron; 
p2.makebox (0.5,0.5,0.5);

p = p - p2;

p.render ()

%%

p = csg.polyhedron; 
p.makebox (1,1,1,0); 
p.rotate ( [0, tau/8, tau/4] );
p.render ()



%%

nodes = [0, 0;
         1, 0;
         0.5, 1 ];

links = [0;1;2;];
         
distance = 1;

p = csg.polyhedron; 
p.makeextrusion (nodes, links, distance);
p.render

%%

nodes = [0, 0;
         1, 1;
         2, 1.5;
         2, 0; ];

links = [0;1;2;3;];

p = csg.polyhedron; 

p.make_surface_of_revolution (nodes, links);

p.render

p.make_surface_of_revolution (nodes, links, pi);

p.render


%% corner radius

p = csg.polyhedron ();

p.makebox (0.5,0.5,0.5,0);

%p.render ()

p2 = csg.polyhedron ();

p2.makesphere (0.5, 1);

%p2.render ()

p_cornerrad = p - p2;

%p_cornerrad.render();   

p3 = csg.polyhedron ();

p3.makebox (0.8,0.8,0.8,1);

p3.difference ( p_cornerrad );

p3.render ();

