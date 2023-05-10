/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 17:36:24 by gsmereka          #+#    #+#             */
/*   Updated: 2023/05/10 17:02:53 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;

	check_args(argc, argv);
	init_data(argv, &data);
	init_philosophers(&data);
	prepare_threads(&data);
	finalize(&data);
}
