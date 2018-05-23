-- phpMyAdmin SQL Dump
-- version 4.8.0
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: May 22, 2018 at 10:05 PM
-- Server version: 5.7.21
-- PHP Version: 7.1.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `traffic_detection`
--

-- --------------------------------------------------------

--
-- Table structure for table `camera`
--

CREATE TABLE `camera` (
  `camera_id` int(10) NOT NULL,
  `url` varchar(255) NOT NULL,
  `street_name` varchar(255) NOT NULL,
  `latitude` varchar(255) NOT NULL,
  `longitude` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `camera`
--

INSERT INTO `camera` (`camera_id`, `url`, `street_name`, `latitude`, `longitude`) VALUES
(1, 'http://35.198.201.38:5000/camera_1', 'Jalan Dago arah Dago Atas', '-6.879313', '107.616435'),
(2, 'http://35.198.201.38:5000/camera_2', 'Jalan Ir.H.Djuanda depan SMAN 1 arah perempatan Dago Merdeka', '-6.895575', '107.612884'),
(3, 'http://35.198.201.38:5000/camera_3', 'Jalan Ir.H.Djuanda depan KFC Dago arah Jalan Merdeka', '-6.901754', '107.611927'),
(4, 'http://35.198.201.38:5000/camera_4', 'Jalan Ir.H.Djuanda depan KFC Dago arah perempatan Dago Merdeka', '-6.901741', '107.611819'),
(5, 'http://35.198.201.38:5000/camera_5', 'Jalan Merdeka depan SDN 11 Banjarsari', '-6.913381', '107.610379'),
(6, 'http://35.198.201.38:5000/camera_6', 'Jalan Pungkur depan ITC Kebon Kelapa', '-6.927415', '107.606491');

-- --------------------------------------------------------

--
-- Table structure for table `density_configuration`
--

CREATE TABLE `density_configuration` (
  `config_id` int(10) NOT NULL,
  `camera_id` int(20) NOT NULL,
  `real_street_width` int(11) NOT NULL,
  `real_street_height` int(5) NOT NULL,
  `mask_points_id` int(5) NOT NULL,
  `edge_threshold` int(5) NOT NULL,
  `low_threshold` int(5) NOT NULL,
  `max_threshold` int(5) NOT NULL,
  `morph_closing_iteration` int(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `density_configuration`
--

INSERT INTO `density_configuration` (`config_id`, `camera_id`, `real_street_width`, `real_street_height`, `mask_points_id`, `edge_threshold`, `low_threshold`, `max_threshold`, `morph_closing_iteration`) VALUES
(1, 1, 8, 12, 1, 1, 0, 200, 7),
(2, 2, 8, 16, 2, 1, 0, 100, 7),
(3, 3, 8, 16, 3, 1, 50, 100, 7),
(4, 4, 8, 18, 4, 1, 50, 100, 7),
(5, 5, 15, 15, 5, 1, 50, 100, 7),
(6, 6, 15, 20, 6, 1, 0, 100, 7);

-- --------------------------------------------------------

--
-- Table structure for table `density_history`
--

CREATE TABLE `density_history` (
  `density_history_id` int(255) NOT NULL,
  `date_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `camera_id` int(20) NOT NULL,
  `density_state` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `log`
--

CREATE TABLE `log` (
  `id` int(20) NOT NULL,
  `camera_id` varchar(5) NOT NULL,
  `time` varchar(20) NOT NULL,
  `concurrency` int(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `street_mask_points`
--

CREATE TABLE `street_mask_points` (
  `mask_points_id` int(10) NOT NULL,
  `x_lb` int(5) NOT NULL,
  `y_lb` int(5) NOT NULL,
  `x_rb` int(5) NOT NULL,
  `y_rb` int(5) NOT NULL,
  `x_rt` int(5) NOT NULL,
  `y_rt` int(5) NOT NULL,
  `x_lt` int(5) NOT NULL,
  `y_lt` int(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `street_mask_points`
--

INSERT INTO `street_mask_points` (`mask_points_id`, `x_lb`, `y_lb`, `x_rb`, `y_rb`, `x_rt`, `y_rt`, `x_lt`, `y_lt`) VALUES
(1, 250, 505, 880, 540, 635, 170, 325, 140),
(2, 365, 295, 545, 275, 703, 490, 300, 540),
(3, 250, 540, 590, 540, 545, 275, 420, 280),
(4, 500, 275, 650, 275, 790, 540, 425, 540),
(5, 280, 543, 960, 540, 650, 275, 355, 280),
(6, 290, 150, 625, 150, 960, 540, 80, 540);

-- --------------------------------------------------------

--
-- Table structure for table `volume_configuration`
--

CREATE TABLE `volume_configuration` (
  `config_id` int(10) NOT NULL,
  `camera_id` int(20) NOT NULL,
  `crossing_line_x0` int(5) NOT NULL,
  `crossing_line_y0` int(5) NOT NULL,
  `crossing_line_x1` int(5) NOT NULL,
  `crossing_line_y1` int(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `volume_configuration`
--

INSERT INTO `volume_configuration` (`config_id`, `camera_id`, `crossing_line_x0`, `crossing_line_y0`, `crossing_line_x1`, `crossing_line_y1`) VALUES
(1, 1, 100, 350, 900, 350),
(2, 2, 350, 350, 620, 350),
(3, 3, 320, 400, 600, 400),
(4, 4, 380, 400, 780, 400),
(5, 5, 320, 400, 780, 400),
(6, 6, 320, 400, 780, 400);

-- --------------------------------------------------------

--
-- Table structure for table `volume_history`
--

CREATE TABLE `volume_history` (
  `volume_history_id` int(255) NOT NULL,
  `date_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `camera_id` int(20) NOT NULL,
  `volume_size` int(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `volume_history`
--

INSERT INTO `volume_history` (`volume_history_id`, `date_time`, `camera_id`, `volume_size`) VALUES
(1, '2018-02-07 19:46:52', 2, 1),
(2, '2018-02-07 19:19:30', 1, 0),
(3, '2018-02-10 05:49:10', 1, 0),
(4, '2018-02-10 05:52:04', 2, 5),
(5, '2018-02-16 14:44:45', 1, 0),
(6, '2018-02-16 14:44:45', 2, 0),
(7, '2018-02-17 16:02:54', 1, 0),
(8, '2018-02-17 16:02:54', 2, 0),
(9, '2018-02-18 13:55:27', 1, 5),
(10, '2018-02-18 13:55:28', 2, 5),
(11, '2018-02-20 21:37:46', 1, 15),
(12, '2018-02-20 21:43:49', 2, 15),
(13, '2018-02-23 15:09:15', 1, 0),
(14, '2018-02-23 16:36:04', 2, 0),
(15, '2018-02-24 15:53:30', 2, 0),
(16, '2018-02-24 15:53:30', 1, 0),
(17, '2018-02-25 16:27:16', 2, 32),
(18, '2018-02-25 16:56:00', 1, 12),
(19, '2018-02-26 15:35:38', 2, 28),
(20, '2018-02-26 15:38:41', 1, 11),
(21, '2018-02-28 10:00:08', 1, 10),
(22, '2018-02-28 10:06:34', 2, 19),
(23, '2018-03-14 06:42:13', 1, 54),
(24, '2018-03-31 17:15:28', 1, 6),
(25, '2018-03-31 17:12:19', 3, 0),
(26, '2018-03-31 17:13:57', 2, 2),
(27, '2018-03-31 17:12:19', 6, 0),
(28, '2018-03-31 17:12:19', 5, 0),
(29, '2018-03-31 17:12:19', 4, 0),
(30, '2018-04-06 15:13:24', 1, 0),
(31, '2018-04-06 15:11:58', 6, 0),
(32, '2018-04-06 15:17:54', 2, 1),
(33, '2018-04-06 15:11:59', 5, 0),
(34, '2018-04-06 15:11:59', 4, 0),
(35, '2018-04-06 15:12:00', 3, 0);

-- --------------------------------------------------------

--
-- Table structure for table `volume_normal`
--

CREATE TABLE `volume_normal` (
  `volume_normal_id` int(255) NOT NULL,
  `camera_id` int(20) NOT NULL,
  `time` time(6) NOT NULL DEFAULT '00:00:00.000000',
  `volume_normal_size` int(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `volume_normal`
--

INSERT INTO `volume_normal` (`volume_normal_id`, `camera_id`, `time`, `volume_normal_size`) VALUES
(1, 1, '00:00:00.000000', 100),
(2, 1, '00:30:00.000000', 100),
(3, 1, '01:00:00.000000', 100),
(4, 1, '01:30:00.000000', 100),
(5, 1, '02:00:00.000000', 100),
(6, 1, '02:30:00.000000', 100),
(7, 1, '03:00:00.000000', 100),
(8, 1, '03:30:00.000000', 100),
(9, 1, '04:00:00.000000', 100),
(10, 1, '04:30:00.000000', 100),
(11, 1, '05:00:00.000000', 100),
(12, 1, '05:30:00.000000', 100),
(13, 1, '06:00:00.000000', 100),
(14, 1, '06:30:00.000000', 100),
(15, 1, '07:00:00.000000', 100),
(16, 1, '07:30:00.000000', 100),
(17, 1, '08:00:00.000000', 100),
(18, 1, '08:30:00.000000', 100),
(19, 1, '09:00:00.000000', 100),
(20, 1, '09:30:00.000000', 100),
(21, 1, '10:00:00.000000', 100),
(22, 1, '10:30:00.000000', 100),
(23, 1, '11:00:00.000000', 100),
(24, 1, '11:30:00.000000', 100),
(25, 1, '12:00:00.000000', 100),
(26, 1, '12:30:00.000000', 100),
(27, 1, '13:00:00.000000', 100),
(28, 1, '13:30:00.000000', 100),
(29, 1, '14:00:00.000000', 100),
(30, 1, '14:30:00.000000', 100),
(31, 1, '15:00:00.000000', 100),
(32, 1, '15:30:00.000000', 100),
(33, 1, '16:00:00.000000', 100),
(34, 1, '16:30:00.000000', 100),
(35, 1, '17:00:00.000000', 100),
(36, 1, '17:30:00.000000', 100),
(37, 1, '18:00:00.000000', 100),
(38, 1, '18:30:00.000000', 100),
(39, 1, '19:00:00.000000', 100),
(40, 1, '19:30:00.000000', 100),
(41, 1, '20:00:00.000000', 100),
(42, 1, '20:30:00.000000', 100),
(43, 1, '21:00:00.000000', 100),
(44, 1, '21:30:00.000000', 100),
(45, 1, '22:00:00.000000', 100),
(46, 1, '22:30:00.000000', 100),
(47, 1, '23:00:00.000000', 100),
(48, 1, '23:30:00.000000', 100),
(49, 2, '00:00:00.000000', 100),
(50, 2, '00:30:00.000000', 100),
(51, 2, '01:00:00.000000', 100),
(52, 2, '01:30:00.000000', 100),
(53, 2, '02:00:00.000000', 100),
(54, 2, '02:30:00.000000', 100),
(55, 2, '03:00:00.000000', 100),
(56, 2, '03:30:00.000000', 100),
(57, 2, '04:00:00.000000', 100),
(58, 2, '04:30:00.000000', 100),
(59, 2, '05:00:00.000000', 100),
(60, 2, '05:30:00.000000', 100),
(61, 2, '06:00:00.000000', 100),
(62, 2, '06:30:00.000000', 100),
(63, 2, '07:00:00.000000', 100),
(64, 2, '07:30:00.000000', 100),
(65, 2, '08:00:00.000000', 100),
(66, 2, '08:30:00.000000', 100),
(67, 2, '09:00:00.000000', 100),
(68, 2, '09:30:00.000000', 100),
(69, 2, '10:00:00.000000', 100),
(70, 2, '10:30:00.000000', 100),
(71, 2, '11:00:00.000000', 100),
(72, 2, '11:30:00.000000', 100),
(73, 2, '12:00:00.000000', 100),
(74, 2, '12:30:00.000000', 100),
(75, 2, '13:00:00.000000', 100),
(76, 2, '13:30:00.000000', 100),
(77, 2, '14:00:00.000000', 100),
(78, 2, '14:30:00.000000', 100),
(79, 2, '15:00:00.000000', 100),
(80, 2, '15:30:00.000000', 100),
(81, 2, '16:00:00.000000', 100),
(82, 2, '16:30:00.000000', 100),
(83, 2, '17:00:00.000000', 100),
(84, 2, '17:30:00.000000', 100),
(85, 2, '18:00:00.000000', 100),
(86, 2, '18:30:00.000000', 100),
(87, 2, '19:00:00.000000', 100),
(88, 2, '19:30:00.000000', 100),
(89, 2, '20:00:00.000000', 100),
(90, 2, '20:30:00.000000', 100),
(91, 2, '21:00:00.000000', 100),
(92, 2, '21:30:00.000000', 100),
(93, 2, '22:00:00.000000', 100),
(94, 2, '22:30:00.000000', 100),
(95, 2, '23:00:00.000000', 100),
(96, 2, '23:30:00.000000', 100),
(97, 3, '00:00:00.000000', 100),
(98, 3, '00:30:00.000000', 100),
(99, 3, '01:00:00.000000', 100),
(100, 3, '01:30:00.000000', 100),
(101, 3, '02:00:00.000000', 100),
(102, 3, '02:30:00.000000', 100),
(103, 3, '03:00:00.000000', 100),
(104, 3, '03:30:00.000000', 100),
(105, 3, '04:00:00.000000', 100),
(106, 3, '04:30:00.000000', 100),
(107, 3, '05:00:00.000000', 100),
(108, 3, '05:30:00.000000', 100),
(109, 3, '06:00:00.000000', 100),
(110, 3, '06:30:00.000000', 100),
(111, 3, '07:00:00.000000', 100),
(112, 3, '07:30:00.000000', 100),
(113, 3, '08:00:00.000000', 100),
(114, 3, '08:30:00.000000', 100),
(115, 3, '09:00:00.000000', 100),
(116, 3, '09:30:00.000000', 100),
(117, 3, '10:00:00.000000', 100),
(118, 3, '10:30:00.000000', 100),
(119, 3, '11:00:00.000000', 100),
(120, 3, '11:30:00.000000', 100),
(121, 3, '12:00:00.000000', 100),
(122, 3, '12:30:00.000000', 100),
(123, 3, '13:00:00.000000', 100),
(124, 3, '13:30:00.000000', 100),
(125, 3, '14:00:00.000000', 100),
(126, 3, '14:30:00.000000', 100),
(127, 3, '15:00:00.000000', 100),
(128, 3, '15:30:00.000000', 100),
(129, 3, '16:00:00.000000', 100),
(130, 3, '16:30:00.000000', 100),
(131, 3, '17:00:00.000000', 100),
(132, 3, '17:30:00.000000', 100),
(133, 3, '18:00:00.000000', 100),
(134, 3, '18:30:00.000000', 100),
(135, 3, '19:00:00.000000', 100),
(136, 3, '19:30:00.000000', 100),
(137, 3, '20:00:00.000000', 100),
(138, 3, '20:30:00.000000', 100),
(139, 3, '21:00:00.000000', 100),
(140, 3, '21:30:00.000000', 100),
(141, 3, '22:00:00.000000', 100),
(142, 3, '22:30:00.000000', 100),
(143, 3, '23:00:00.000000', 100),
(144, 3, '23:30:00.000000', 100),
(145, 4, '00:00:00.000000', 100),
(146, 4, '00:30:00.000000', 100),
(147, 4, '01:00:00.000000', 100),
(148, 4, '01:30:00.000000', 100),
(149, 4, '02:00:00.000000', 100),
(150, 4, '02:30:00.000000', 100),
(151, 4, '03:00:00.000000', 100),
(152, 4, '03:30:00.000000', 100),
(153, 4, '04:00:00.000000', 100),
(154, 4, '04:30:00.000000', 100),
(155, 4, '05:00:00.000000', 100),
(156, 4, '05:30:00.000000', 100),
(157, 4, '06:00:00.000000', 100),
(158, 4, '06:30:00.000000', 100),
(159, 4, '07:00:00.000000', 100),
(160, 4, '07:30:00.000000', 100),
(161, 4, '08:00:00.000000', 100),
(162, 4, '08:30:00.000000', 100),
(163, 4, '09:00:00.000000', 100),
(164, 4, '09:30:00.000000', 100),
(165, 4, '10:00:00.000000', 100),
(166, 4, '10:30:00.000000', 100),
(167, 4, '11:00:00.000000', 100),
(168, 4, '11:30:00.000000', 100),
(169, 4, '12:00:00.000000', 100),
(170, 4, '12:30:00.000000', 100),
(171, 4, '13:00:00.000000', 100),
(172, 4, '13:30:00.000000', 100),
(173, 4, '14:00:00.000000', 100),
(174, 4, '14:30:00.000000', 100),
(175, 4, '15:00:00.000000', 100),
(176, 4, '15:30:00.000000', 100),
(177, 4, '16:00:00.000000', 100),
(178, 4, '16:30:00.000000', 100),
(179, 4, '17:00:00.000000', 100),
(180, 4, '17:30:00.000000', 100),
(181, 4, '18:00:00.000000', 100),
(182, 4, '18:30:00.000000', 100),
(183, 4, '19:00:00.000000', 100),
(184, 4, '19:30:00.000000', 100),
(185, 4, '20:00:00.000000', 100),
(186, 4, '20:30:00.000000', 100),
(187, 4, '21:00:00.000000', 100),
(188, 4, '21:30:00.000000', 100),
(189, 4, '22:00:00.000000', 100),
(190, 4, '22:30:00.000000', 100),
(191, 4, '23:00:00.000000', 100),
(192, 4, '23:30:00.000000', 100),
(193, 5, '00:00:00.000000', 100),
(194, 5, '00:30:00.000000', 100),
(195, 5, '01:00:00.000000', 100),
(196, 5, '01:30:00.000000', 100),
(197, 5, '02:00:00.000000', 100),
(198, 5, '02:30:00.000000', 100),
(199, 5, '03:00:00.000000', 100),
(200, 5, '03:30:00.000000', 100),
(201, 5, '04:00:00.000000', 100),
(202, 5, '04:30:00.000000', 100),
(203, 5, '05:00:00.000000', 100),
(204, 5, '05:30:00.000000', 100),
(205, 5, '06:00:00.000000', 100),
(206, 5, '06:30:00.000000', 100),
(207, 5, '07:00:00.000000', 100),
(208, 5, '07:30:00.000000', 100),
(209, 5, '08:00:00.000000', 100),
(210, 5, '08:30:00.000000', 100),
(211, 5, '09:00:00.000000', 100),
(212, 5, '09:30:00.000000', 100),
(213, 5, '10:00:00.000000', 100),
(214, 5, '10:30:00.000000', 100),
(215, 5, '11:00:00.000000', 100),
(216, 5, '11:30:00.000000', 100),
(217, 5, '12:00:00.000000', 100),
(218, 5, '12:30:00.000000', 100),
(219, 5, '13:00:00.000000', 100),
(220, 5, '13:30:00.000000', 100),
(221, 5, '14:00:00.000000', 100),
(222, 5, '14:30:00.000000', 100),
(223, 5, '15:00:00.000000', 100),
(224, 5, '15:30:00.000000', 100),
(225, 5, '16:00:00.000000', 100),
(226, 5, '16:30:00.000000', 100),
(227, 5, '17:00:00.000000', 100),
(228, 5, '17:30:00.000000', 100),
(229, 5, '18:00:00.000000', 100),
(230, 5, '18:30:00.000000', 100),
(231, 5, '19:00:00.000000', 100),
(232, 5, '19:30:00.000000', 100),
(233, 5, '20:00:00.000000', 100),
(234, 5, '20:30:00.000000', 100),
(235, 5, '21:00:00.000000', 100),
(236, 5, '21:30:00.000000', 100),
(237, 5, '22:00:00.000000', 100),
(238, 5, '22:30:00.000000', 100),
(239, 5, '23:00:00.000000', 100),
(240, 5, '23:30:00.000000', 100),
(241, 6, '00:00:00.000000', 100),
(242, 6, '00:30:00.000000', 100),
(243, 6, '01:00:00.000000', 100),
(244, 6, '01:30:00.000000', 100),
(245, 6, '02:00:00.000000', 100),
(246, 6, '02:30:00.000000', 100),
(247, 6, '03:00:00.000000', 100),
(248, 6, '03:30:00.000000', 100),
(249, 6, '04:00:00.000000', 100),
(250, 6, '04:30:00.000000', 100),
(251, 6, '05:00:00.000000', 100),
(252, 6, '05:30:00.000000', 100),
(253, 6, '06:00:00.000000', 100),
(254, 6, '06:30:00.000000', 100),
(255, 6, '07:00:00.000000', 100),
(256, 6, '07:30:00.000000', 100),
(257, 6, '08:00:00.000000', 100),
(258, 6, '08:30:00.000000', 100),
(259, 6, '09:00:00.000000', 100),
(260, 6, '09:30:00.000000', 100),
(261, 6, '10:00:00.000000', 100),
(262, 6, '10:30:00.000000', 100),
(263, 6, '11:00:00.000000', 100),
(264, 6, '11:30:00.000000', 100),
(265, 6, '12:00:00.000000', 100),
(266, 6, '12:30:00.000000', 100),
(267, 6, '13:00:00.000000', 100),
(268, 6, '13:30:00.000000', 100),
(269, 6, '14:00:00.000000', 100),
(270, 6, '14:30:00.000000', 100),
(271, 6, '15:00:00.000000', 100),
(272, 6, '15:30:00.000000', 100),
(273, 6, '16:00:00.000000', 100),
(274, 6, '16:30:00.000000', 100),
(275, 6, '17:00:00.000000', 100),
(276, 6, '17:30:00.000000', 100),
(277, 6, '18:00:00.000000', 100),
(278, 6, '18:30:00.000000', 100),
(279, 6, '19:00:00.000000', 100),
(280, 6, '19:30:00.000000', 100),
(281, 6, '20:00:00.000000', 100),
(282, 6, '20:30:00.000000', 100),
(283, 6, '21:00:00.000000', 100),
(284, 6, '21:30:00.000000', 100),
(285, 6, '22:00:00.000000', 100),
(286, 6, '22:30:00.000000', 100),
(287, 6, '23:00:00.000000', 100),
(288, 6, '23:30:00.000000', 100);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `camera`
--
ALTER TABLE `camera`
  ADD PRIMARY KEY (`camera_id`);

--
-- Indexes for table `density_configuration`
--
ALTER TABLE `density_configuration`
  ADD PRIMARY KEY (`config_id`),
  ADD KEY `mask_points_id` (`mask_points_id`);

--
-- Indexes for table `density_history`
--
ALTER TABLE `density_history`
  ADD PRIMARY KEY (`density_history_id`);

--
-- Indexes for table `log`
--
ALTER TABLE `log`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `street_mask_points`
--
ALTER TABLE `street_mask_points`
  ADD PRIMARY KEY (`mask_points_id`);

--
-- Indexes for table `volume_configuration`
--
ALTER TABLE `volume_configuration`
  ADD PRIMARY KEY (`config_id`);

--
-- Indexes for table `volume_history`
--
ALTER TABLE `volume_history`
  ADD PRIMARY KEY (`volume_history_id`);

--
-- Indexes for table `volume_normal`
--
ALTER TABLE `volume_normal`
  ADD PRIMARY KEY (`volume_normal_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `camera`
--
ALTER TABLE `camera`
  MODIFY `camera_id` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `density_configuration`
--
ALTER TABLE `density_configuration`
  MODIFY `config_id` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `density_history`
--
ALTER TABLE `density_history`
  MODIFY `density_history_id` int(255) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `log`
--
ALTER TABLE `log`
  MODIFY `id` int(20) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `street_mask_points`
--
ALTER TABLE `street_mask_points`
  MODIFY `mask_points_id` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `volume_configuration`
--
ALTER TABLE `volume_configuration`
  MODIFY `config_id` int(10) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `volume_history`
--
ALTER TABLE `volume_history`
  MODIFY `volume_history_id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=36;

--
-- AUTO_INCREMENT for table `volume_normal`
--
ALTER TABLE `volume_normal`
  MODIFY `volume_normal_id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=289;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `density_configuration`
--
ALTER TABLE `density_configuration`
  ADD CONSTRAINT `density_configuration_ibfk_1` FOREIGN KEY (`mask_points_id`) REFERENCES `street_mask_points` (`mask_points_id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
