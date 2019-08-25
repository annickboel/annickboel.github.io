CREATE TABLE IF NOT EXISTS `markers` (
  `id` int(11) NOT NULL,
  `label` varchar(200) NOT NULL,
  `codepos` int(5) NOT NULL,
  `city` varchar(20) NOT NULL,
  `lat` DECIMAL(10, 8) NOT NULL,
  `lng` DECIMAL(11, 8) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
);
 
ALTER TABLE `markers` ADD PRIMARY KEY (`id`);
ALTER TABLE `markers` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
