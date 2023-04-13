
--una tabla personas
-- una tabla de hobbies
-- una tabla de amigos

--los nombres del par de personas que se conocen
--los nombres del par de personas que se conocen y que comparten el mismo hobbie



CREATE TABLE persons (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    fullname TEXT,
    age INTEGER);
    
INSERT INTO persons (fullname, age) VALUES ("Bobby McBobbyFace", "12");
INSERT INTO persons (fullname, age) VALUES ("Lucy BoBucie", "25");
INSERT INTO persons (fullname, age) VALUES ("Banana FoFanna", "14");
INSERT INTO persons (fullname, age) VALUES ("Shish Kabob", "20");
INSERT INTO persons (fullname, age) VALUES ("Fluffy Sparkles", "8");

CREATE table hobbies (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    person_id INTEGER,
    name TEXT);
INSERT INTO hobbies (person_id, name) VALUES (2, "drawing");    
INSERT INTO hobbies (person_id, name) VALUES (1, "drawing");
INSERT INTO hobbies (person_id, name) VALUES (1, "coding");
INSERT INTO hobbies (person_id, name) VALUES (2, "dancing");
INSERT INTO hobbies (person_id, name) VALUES (2, "coding");
INSERT INTO hobbies (person_id, name) VALUES (3, "skating");
INSERT INTO hobbies (person_id, name) VALUES (3, "rowing");
INSERT INTO hobbies (person_id, name) VALUES (3, "drawing");
INSERT INTO hobbies (person_id, name) VALUES (4, "coding");
INSERT INTO hobbies (person_id, name) VALUES (4, "dilly-dallying");
INSERT INTO hobbies (person_id, name) VALUES (4, "meowing");

CREATE table friends (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    person1_id INTEGER,
    person2_id INTEGER);

INSERT INTO friends (person1_id, person2_id)
    VALUES (1, 4);
INSERT INTO friends (person1_id, person2_id)
    VALUES (2, 3);
INSERT INTO friends (person1_id, person2_id)
    VALUES (1, 3);    
INSERT INTO friends (person1_id, person2_id)
    VALUES (1, 2);



--los nombres del par de personas que se conocen
select p1.fullname as amigo1, p2.fullname as amigo2 from friends
    join persons as p1 
        on person1_id = p1.id
    join persons as p2
        on person2_id = p2.id;


--los nombres del par de personas que se conocen y que comparten el mismo hobbie
with mismo_hobbie as (
select  h1.person_id as id1, 
        h2.person_id as id2, 
        h1.name   
from friends 
    join hobbies as h1
        on person1_id = h1.person_id
    join hobbies as h2
        on person2_id = h2.person_id
where h1.name = h2.name
   )

select f1.fullname as amigo1, 
       f2.fullname as amigo2, 
       mh.name as hobbie 
from mismo_hobbie as mh
    join persons as f1
        on mh.id1 = f1.id
    join persons as f2
        on mh.id2 = f2.id;


--cuantos pares de amigos hay que comparten un mismo hobbie
with mismo_hobbie as (
select  h1.person_id as id1, h2.person_id as id2, h1.name   from friends 
    join hobbies as h1
        on person1_id = h1.person_id
    join hobbies as h2
        on person2_id = h2.person_id
    where h1.name = h2.name
   )
    
select mh.name as hobbie, count(mh.id1) as count from mismo_hobbie as mh
    join persons as f1
        on mh.id1 = f1.id
    join persons as f2
        on mh.id2 = f2.id
group by mh.name;




--que personas no son amigos entre si