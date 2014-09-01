


p = csg.polyhedron;
p.makebox(1,1,1);
[nodes, vertinds] = p.triangulate ();
p.render ();

n = p.num_faces ();
verts = [];
facecoords = [];

%for face_id = 0:n-1
%    
%    verts = [verts; p.get_face_vertices (face_id)];
%    
%    for indii = 1:numel(verts)
%    
%        facecoords = [ facecoords; p.get_vertex(verts(faceid+1,indii)) ];
%    
%    end
%
%end

if exist ('surf2mesh')
    [node,elem,face] = surf2mesh(nodes, vertinds+1, [-1 -1 -1], [1 1 1], 0.9, 0.001, [0,0,0]); %,p0,p1,keepratio,maxvol,regions,holes,forcebox)

    plotmesh(node,face(:,1:3));
    axis equal;
end


%%

p = csg.polyhedron;
p.makebox(1,1,1);
[fv.vertices, fv.faces] = p.triangulate ();
fv.faces = fv.faces + 1;

stlwrite('test.stl',fv) 

%%

clear p;
p = csg.polyhedron;
p.makebox(1,1,1);

p.freecadwrite (1);


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
p.make_extrusion (distance, nodes, links );
p.render

%%
nodes = [0, 0;
         1, 0;
         0.5, 1 ];

links = [0;1;2;];
         
distance = 1;
segments = 100;
dTheta = 360 / segments;

p = csg.polyhedron; 
p.extrude_rotate (distance, segments, dTheta, nodes, links );
p.render

%%

p1 = csg.polyhedron; 
p1.makebox (1,1,1,0); 
p1.rotate ( [0, tau/8, tau/4] );

nodes = [0, 0;
         1, 0;
         0.5, 1 ];

links = [0;1;2;];
         
distance = 1;

p2 = csg.polyhedron; 
p2.make_extrusion (distance, nodes, links );

p2.union(p1);

p2.render

%%

nodes = [0, 0.1;
         1, 1;
         2, 1.5;
         2, 0.1; ];

links = [0;1;2;3;];

p1 = csg.polyhedron; 

p1.make_surface_of_revolution (nodes, links, pi);
% 
p1.render

p2 = csg.polyhedron; 
p2.make_surface_of_revolution (nodes, links, pi);

p2.translate ([0.7,0,0])

% p2.render 

p2.union (p1);

p3 = csg.polyhedron; 
p3.make_surface_of_revolution (nodes, links, pi);

p3.translate ([1.4,0,0])

p2.union (p3);
p2.render


%% corner radius

p = csg.polyhedron ();

p.makebox (0.6,0.6,0.6,0);

p.render ()

% p3.makebox (0.5,0.5,0.5,0);

%p.render ()

% p2 = csg.polyhedron ();
% p2.makecylinder (0.5, 0.3, 1)
% p2.render ();

% p2 = csg.cube (0.3, 0.3, 0.3, 0);
% p2.render ();

p2 = csg.sphere (0.5, 'segx', 20, 'segy', 20);
%  p2.rotate ([90, 0, 0])

% p2.makesphere (0.5, 1, 37);

p2.render ()

p_cornerrad = p - p2;

% p_cornerrad = p2;

p_cornerrad.render();   

p3 = csg.polyhedron ();

p3.makebox (0.6,0.6,0.6,0);

% p3.makebox (0.8,0.8,0.8,1);

p3.difference ( p_cornerrad );

p3.render ();


nodes = p_cornerrad.get_vertices ()

[u,I,J] = unique(nodes, 'rows', 'first');
hasDuplicates = size(u,1) < size(nodes,1)
ixDupRows = setdiff(1:size(nodes,1), I)
dupRowValues = nodes(ixDupRows,:)

