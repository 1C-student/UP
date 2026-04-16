--
-- PostgreSQL database dump
--

\restrict p1xgkM7MayldbCoUcpDqTDpCqk0LU03nKo1MZinHt6JlyXJXnT9620kyfgOhTID

-- Dumped from database version 18.0
-- Dumped by pg_dump version 18.0

-- Started on 2026-04-16 23:21:02

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 220 (class 1259 OID 26991)
-- Name: cities; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.cities (
    id integer NOT NULL,
    name character varying(100) NOT NULL
);


ALTER TABLE public.cities OWNER TO postgre;

--
-- TOC entry 219 (class 1259 OID 26990)
-- Name: cities_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.cities_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.cities_id_seq OWNER TO postgre;

--
-- TOC entry 4936 (class 0 OID 0)
-- Dependencies: 219
-- Name: cities_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.cities_id_seq OWNED BY public.cities.id;


--
-- TOC entry 226 (class 1259 OID 27029)
-- Name: event_log; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.event_log (
    id integer NOT NULL,
    member_id integer NOT NULL,
    event_id integer NOT NULL,
    visit_date date DEFAULT CURRENT_DATE NOT NULL
);


ALTER TABLE public.event_log OWNER TO postgre;

--
-- TOC entry 225 (class 1259 OID 27028)
-- Name: event_log_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.event_log_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.event_log_id_seq OWNER TO postgre;

--
-- TOC entry 4937 (class 0 OID 0)
-- Dependencies: 225
-- Name: event_log_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.event_log_id_seq OWNED BY public.event_log.id;


--
-- TOC entry 224 (class 1259 OID 27019)
-- Name: events; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.events (
    id integer NOT NULL,
    name character varying(200) NOT NULL,
    event_date date NOT NULL
);


ALTER TABLE public.events OWNER TO postgre;

--
-- TOC entry 223 (class 1259 OID 27018)
-- Name: events_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.events_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.events_id_seq OWNER TO postgre;

--
-- TOC entry 4938 (class 0 OID 0)
-- Dependencies: 223
-- Name: events_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.events_id_seq OWNED BY public.events.id;


--
-- TOC entry 222 (class 1259 OID 27002)
-- Name: members; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.members (
    id integer NOT NULL,
    full_name character varying(200) NOT NULL,
    photo_path character varying(500),
    city_id integer NOT NULL
);


ALTER TABLE public.members OWNER TO postgre;

--
-- TOC entry 221 (class 1259 OID 27001)
-- Name: members_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.members_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.members_id_seq OWNER TO postgre;

--
-- TOC entry 4939 (class 0 OID 0)
-- Dependencies: 221
-- Name: members_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.members_id_seq OWNED BY public.members.id;


--
-- TOC entry 228 (class 1259 OID 27053)
-- Name: users; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.users (
    id integer NOT NULL,
    login character varying(50) NOT NULL,
    password character varying(100) NOT NULL,
    role character varying(20) NOT NULL,
    CONSTRAINT users_role_check CHECK (((role)::text = ANY ((ARRAY['admin'::character varying, 'user'::character varying])::text[])))
);


ALTER TABLE public.users OWNER TO postgre;

--
-- TOC entry 227 (class 1259 OID 27052)
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.users_id_seq OWNER TO postgre;

--
-- TOC entry 4940 (class 0 OID 0)
-- Dependencies: 227
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 4745 (class 2604 OID 26994)
-- Name: cities id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.cities ALTER COLUMN id SET DEFAULT nextval('public.cities_id_seq'::regclass);


--
-- TOC entry 4748 (class 2604 OID 27032)
-- Name: event_log id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.event_log ALTER COLUMN id SET DEFAULT nextval('public.event_log_id_seq'::regclass);


--
-- TOC entry 4747 (class 2604 OID 27022)
-- Name: events id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.events ALTER COLUMN id SET DEFAULT nextval('public.events_id_seq'::regclass);


--
-- TOC entry 4746 (class 2604 OID 27005)
-- Name: members id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.members ALTER COLUMN id SET DEFAULT nextval('public.members_id_seq'::regclass);


--
-- TOC entry 4750 (class 2604 OID 27056)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 4922 (class 0 OID 26991)
-- Dependencies: 220
-- Data for Name: cities; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.cities (id, name) FROM stdin;
1	Москва
2	Санкт-Петербург
3	Саратов
4	Казань
5	Новосибирск
6	Екатеринбург
7	Феодосия
8	Ялта
\.


--
-- TOC entry 4928 (class 0 OID 27029)
-- Dependencies: 226
-- Data for Name: event_log; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.event_log (id, member_id, event_id, visit_date) FROM stdin;
3	8	3	2026-03-31
1	4	2	2026-03-24
5	6	2	2026-03-31
6	7	4	2026-03-18
7	1	2	2026-03-31
10	1	1	2026-04-03
11	1	3	2026-04-03
13	3	1	2026-05-09
14	6	1	2026-04-03
\.


--
-- TOC entry 4926 (class 0 OID 27019)
-- Dependencies: 224
-- Data for Name: events; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.events (id, name, event_date) FROM stdin;
1	Отчётно-выборный съезд	2025-12-15
2	Заседание Генсовета	2026-01-10
3	Предварительное голосование	2026-05-22
4	Собрание	2026-03-04
6	Участок	2026-05-01
\.


--
-- TOC entry 4924 (class 0 OID 27002)
-- Dependencies: 222
-- Data for Name: members; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.members (id, full_name, photo_path, city_id) FROM stdin;
2	Медведев Дмитрий Анатольевич	photos/medvedev.jpg	1
3	Мишустин Михаил Владимирович	photos/mishustin.jpg	1
4	Шойгу Сергей Кужугетович	photos/shoygu.jpg	1
5	Лавров Сергей Викторович	photos/lavrov.jpg	1
6	Володин Вячеслав Викторович	photos/volodin.jpg	3
7	Матвиенко Валентина Ивановна	photos/matvienko.jpg	2
8	Собянин Сергей Семёнович	photos/sobyanin.jpg	1
9	Патрушев Николай Платонович	photos/patrushev.jpg	1
1	Путин Владимир Владимирович	photos/putin.jpg	1
\.


--
-- TOC entry 4930 (class 0 OID 27053)
-- Dependencies: 228
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.users (id, login, password, role) FROM stdin;
1	putin	21232f297a57a5a743894a0e4a801fc3	user
2	medvedev	21232f297a57a5a743894a0e4a801fc3	user
3	mishustin	21232f297a57a5a743894a0e4a801fc3	user
4	shoygu	21232f297a57a5a743894a0e4a801fc3	user
5	lavrov	21232f297a57a5a743894a0e4a801fc3	user
6	volodin	21232f297a57a5a743894a0e4a801fc3	user
7	matvienko	21232f297a57a5a743894a0e4a801fc3	user
8	sobyanin	21232f297a57a5a743894a0e4a801fc3	user
9	patrushev	21232f297a57a5a743894a0e4a801fc3	user
10	naryshkin	21232f297a57a5a743894a0e4a801fc3	user
11	admin	21232f297a57a5a743894a0e4a801fc3	admin
12	kirill	59ccf2f4c0304c4640db256c821efca8	user
13	kir	59ccf2f4c0304c4640db256c821efca8	user
\.


--
-- TOC entry 4941 (class 0 OID 0)
-- Dependencies: 219
-- Name: cities_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.cities_id_seq', 9, true);


--
-- TOC entry 4942 (class 0 OID 0)
-- Dependencies: 225
-- Name: event_log_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.event_log_id_seq', 14, true);


--
-- TOC entry 4943 (class 0 OID 0)
-- Dependencies: 223
-- Name: events_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.events_id_seq', 7, true);


--
-- TOC entry 4944 (class 0 OID 0)
-- Dependencies: 221
-- Name: members_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.members_id_seq', 12, true);


--
-- TOC entry 4945 (class 0 OID 0)
-- Dependencies: 227
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.users_id_seq', 13, true);


--
-- TOC entry 4753 (class 2606 OID 27000)
-- Name: cities cities_name_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.cities
    ADD CONSTRAINT cities_name_key UNIQUE (name);


--
-- TOC entry 4755 (class 2606 OID 26998)
-- Name: cities cities_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.cities
    ADD CONSTRAINT cities_pkey PRIMARY KEY (id);


--
-- TOC entry 4762 (class 2606 OID 27041)
-- Name: event_log event_log_member_id_event_id_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.event_log
    ADD CONSTRAINT event_log_member_id_event_id_key UNIQUE (member_id, event_id);


--
-- TOC entry 4764 (class 2606 OID 27039)
-- Name: event_log event_log_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.event_log
    ADD CONSTRAINT event_log_pkey PRIMARY KEY (id);


--
-- TOC entry 4760 (class 2606 OID 27027)
-- Name: events events_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.events
    ADD CONSTRAINT events_pkey PRIMARY KEY (id);


--
-- TOC entry 4758 (class 2606 OID 27012)
-- Name: members members_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.members
    ADD CONSTRAINT members_pkey PRIMARY KEY (id);


--
-- TOC entry 4768 (class 2606 OID 27065)
-- Name: users users_login_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_login_key UNIQUE (login);


--
-- TOC entry 4770 (class 2606 OID 27063)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 4765 (class 1259 OID 27068)
-- Name: idx_event_log_event; Type: INDEX; Schema: public; Owner: postgre
--

CREATE INDEX idx_event_log_event ON public.event_log USING btree (event_id);


--
-- TOC entry 4766 (class 1259 OID 27067)
-- Name: idx_event_log_member; Type: INDEX; Schema: public; Owner: postgre
--

CREATE INDEX idx_event_log_member ON public.event_log USING btree (member_id);


--
-- TOC entry 4756 (class 1259 OID 27066)
-- Name: idx_members_city; Type: INDEX; Schema: public; Owner: postgre
--

CREATE INDEX idx_members_city ON public.members USING btree (city_id);


--
-- TOC entry 4772 (class 2606 OID 27047)
-- Name: event_log event_log_event_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.event_log
    ADD CONSTRAINT event_log_event_id_fkey FOREIGN KEY (event_id) REFERENCES public.events(id) ON DELETE CASCADE;


--
-- TOC entry 4773 (class 2606 OID 27042)
-- Name: event_log event_log_member_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.event_log
    ADD CONSTRAINT event_log_member_id_fkey FOREIGN KEY (member_id) REFERENCES public.members(id) ON DELETE CASCADE;


--
-- TOC entry 4771 (class 2606 OID 27013)
-- Name: members members_city_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.members
    ADD CONSTRAINT members_city_id_fkey FOREIGN KEY (city_id) REFERENCES public.cities(id) ON DELETE RESTRICT;


-- Completed on 2026-04-16 23:21:02

--
-- PostgreSQL database dump complete
--

\unrestrict p1xgkM7MayldbCoUcpDqTDpCqk0LU03nKo1MZinHt6JlyXJXnT9620kyfgOhTID

